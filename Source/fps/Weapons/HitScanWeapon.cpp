// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"
#include "DrawDebugHelpers.h"
#include "../Actors/FpsCharacter.h"
#include "Camera/CameraComponent.h"

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
