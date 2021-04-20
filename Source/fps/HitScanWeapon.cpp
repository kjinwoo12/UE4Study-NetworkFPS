// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"
#include "DrawDebugHelpers.h"
#include "FPSCharacter.h"

AHitScanWeapon::AHitScanWeapon()
{
}

void AHitScanWeapon::Initialize(AFPSCharacter* FPSCharacter)
{
	AWeaponBase::Initialize(FPSCharacter);
	LineTraceCollisionQueryParams.AddIgnoredActor(FPSCharacter);
}

void AHitScanWeapon::OnUnEquipped()
{
	Super::OnUnEquipped();
	LineTraceCollisionQueryParams.ClearIgnoredActors();
}

void AHitScanWeapon::OnAction()
{
	Super::OnAction();
	FHitResult HitResult;
	if (!LineTrace(HitResult)) return;
	FPointDamageEvent DamangeEvent;
	DamangeEvent.HitInfo = HitResult;

	HitResult.GetActor()->TakeDamage(Damage, DamangeEvent, PlayerController, this);
}

bool AHitScanWeapon::LineTrace(FHitResult& HitResult)
{
	// Get Player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	PlayerController->GetPlayerViewPoint(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);

	// Get end point
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	// Get Muzzle point
	FVector MuzzleLocation = Muzzle->GetComponentLocation();

	//It is for checking line. Player view point to end point
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		5.f,
		0.f,
		1.f
	);

	bool IsHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		PlayerViewPointLocation,
		LineTraceEnd,
		ECollisionChannel::ECC_Pawn,
		LineTraceCollisionQueryParams
	);

	if (!IsHit)
	{
		DrawDebugLine(
			GetWorld(),
			MuzzleLocation,
			LineTraceEnd,
			FColor(0, 255, 0),
			false,
			5.f,
			0.f,
			1.f
		);
	}
	else
	{
		DrawDebugLine(
			GetWorld(),
			MuzzleLocation,
			HitResult.ImpactPoint,
			FColor(0, 255, 0),
			false,
			5.f,
			0.f,
			1.f
		);
	}

	return IsHit;
}