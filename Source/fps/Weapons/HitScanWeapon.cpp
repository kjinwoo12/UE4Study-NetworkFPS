// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"
#include "DrawDebugHelpers.h"
#include "../Actors/FpsCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	FHitResult HitResult;
	if (LineTrace(HitResult))
	{
		FPointDamageEvent DamangeEvent;
		DamangeEvent.HitInfo = HitResult;

		AFpsCharacter* Character = Cast<AFpsCharacter>(GetOwner());
		if (!IsValid(Character)) 
		{
			UE_LOG(LogTemp, Log, TEXT("AHitScanWeapon::OnAction / Owner Character is invalid"));
			return;
		}
		APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		if (!IsValid(PlayerController))
		{
			UE_LOG(LogTemp, Log, TEXT("AHitScanWeapon::OnAction / PlayerController is invalid"));
			return;
		}
		HitResult.GetActor()->TakeDamage(Damage, DamangeEvent, PlayerController, this);
	}
}

void AHitScanWeapon::OnBulletRecoilProgress(FVector Recoil)
{
	Super::OnBulletRecoilProgress(Recoil);
	BulletRecoil = Recoil;
}

bool AHitScanWeapon::LineTrace(FHitResult& HitResult)
{
	AFpsCharacter* Character = Cast<AFpsCharacter>(GetOwner());
	if (!IsValid(Character))
	{
		UE_LOG(LogTemp, Log, TEXT("AHitScanWeapon::LineTrace / Owner Character is invalid"));
		return false;
	}
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Log, TEXT("AHitScanWeapon::LineTrace / PlayerController is invalid"));
		return false;
	}

	// Get Player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	PlayerController->GetPlayerViewPoint(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);

	//Recoil
	float RecoilOffset = GetRecoilOffset(); // Accuracy + MovementStability;
	PlayerViewPointRotation.Pitch += BulletRecoil.Y;
	PlayerViewPointRotation.Pitch = FMath::RandRange(PlayerViewPointRotation.Pitch, PlayerViewPointRotation.Pitch + RecoilOffset);
	PlayerViewPointRotation.Yaw += BulletRecoil.Z;
	PlayerViewPointRotation.Yaw = FMath::RandRange(PlayerViewPointRotation.Yaw - RecoilOffset, PlayerViewPointRotation.Yaw + RecoilOffset);
	PlayerViewPointRotation.Roll += BulletRecoil.X;

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