// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"
#include "DrawDebugHelpers.h"
#include "../Actors/FpsCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"

AHitScanWeapon::AHitScanWeapon() : AWeaponBase()
{
	Reach = 6000.f;
}

void AHitScanWeapon::Initialize(AFpsCharacter* FpsCharacter)
{
	AWeaponBase::Initialize(FpsCharacter);
	LineTraceCollisionQueryParams.AddIgnoredActor(FpsCharacter);
}

void AHitScanWeapon::OnUnequipHands(AHands* Hands)
{
	Super::OnUnequipHands(Hands);
	LineTraceCollisionQueryParams.ClearIgnoredActors();
}

void AHitScanWeapon::OnAction()
{
	Super::OnAction();
	UE_LOG(LogTemp, Log, TEXT("HitScanWeapon OnAction"));
	if (CurrentAmmo <= 0) return;

	AFpsCharacter* Character = Cast<AFpsCharacter>(GetOwner());
	if (!IsValid(Character))
	{
		UE_LOG(LogTemp, Log, TEXT("AHitScanWeapon::LineTrace / Owner Character is invalid"));
		return;
	}
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Log, TEXT("AHitScanWeapon::LineTrace / PlayerController is invalid"));
		return;
	}

	
	UpdatePlayerViewPoint(PlayerController);
	UpdateRecoilOffset();

	FHitResult HitResult;
	if (LineTrace(HitResult))
	{
		OnHitResult(HitResult, PlayerController);
	}
}

void AHitScanWeapon::OnHitResult(FHitResult HitResult, APlayerController* PlayerController)
{
	GiveDamage(HitResult, PlayerController);
	MulticastRpcSpawnDecalActor(HitResult.Location, HitResult.ImpactNormal.Rotation());
}

void AHitScanWeapon::GiveDamage(FHitResult HitResult, APlayerController* PlayerController)
{
	FPointDamageEvent DamangeEvent;
	DamangeEvent.HitInfo = HitResult;
	AActor* HitActor = HitResult.GetActor();
	HitActor->TakeDamage(Damage, DamangeEvent, PlayerController, this);
}

void AHitScanWeapon::MulticastRpcSpawnDecalActor_Implementation(FVector Location, FRotator Rotator)
{
	if (DecalMaterial == nullptr || GetNetMode() != ENetMode::NM_Client) return;
	Rotator.Pitch += 90;
	ADecalActor* DecalActor = GetWorld()->SpawnActor<ADecalActor>(Location, Rotator);
	if (!IsValid(DecalActor)) 
	{
		UE_LOG(LogTemp, Log, TEXT("AHitScanWeapon::SpawnDecalActor DecalActor is invalid"));
		return;
	}
	DecalActor->SetDecalMaterial(DecalMaterial);
	DecalActor->SetLifeSpan(5.f);
	DecalActor->GetDecal()->DecalSize = FVector(8.f, 8.f, 8.f);
}

void AHitScanWeapon::UpdatePlayerViewPoint(APlayerController* PlayerController)
{
	PlayerController->GetPlayerViewPoint(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);
}

void AHitScanWeapon::UpdateRecoilOffset()
{
	float RecoilOffset = GetRecoilOffset(); // Accuracy + MovementStability;
	PlayerViewPointRotation.Pitch += CurrentBulletRecoil.Y;
	PlayerViewPointRotation.Pitch = FMath::RandRange(PlayerViewPointRotation.Pitch, PlayerViewPointRotation.Pitch + RecoilOffset);
	PlayerViewPointRotation.Yaw += CurrentBulletRecoil.Z;
	PlayerViewPointRotation.Yaw = FMath::RandRange(PlayerViewPointRotation.Yaw - RecoilOffset, PlayerViewPointRotation.Yaw + RecoilOffset);
	PlayerViewPointRotation.Roll += CurrentBulletRecoil.X;
}

bool AHitScanWeapon::LineTrace(FHitResult& HitResult)
{
	// Get end point
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	bool IsHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		PlayerViewPointLocation,
		LineTraceEnd,
		ECollisionChannel::ECC_Pawn,
		LineTraceCollisionQueryParams
	);

	if (!IsHit)
	{
		HitResult.ImpactPoint = LineTraceEnd;
	}

	return IsHit;
}


float AHitScanWeapon::GetRecoilOffset()
{
	AFpsCharacter* FpsCharacter = Cast<AFpsCharacter>(GetOwner());
	if (!IsValid(FpsCharacter)) return 0.f;

	float CharacterSpeed = FpsCharacter->GetVelocity().Size();
	UCharacterMovementComponent* MovementComponent = FpsCharacter->GetCharacterMovement();
	return Accuracy * (CurrentRecoilRecoveryTime / RecoilRecoveryTime) + MovementStability * (CharacterSpeed / MovementComponent->MaxWalkSpeed);
}