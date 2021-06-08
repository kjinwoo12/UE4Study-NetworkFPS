// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"
#include "DrawDebugHelpers.h"
#include "FPSCharacter.h"
#include "Camera/CameraComponent.h"

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
	UE_LOG(LogTemp, Log, TEXT("HitScanWeapon OnAction"));
	if (CurrentAmmo <= 0) return;

	FHitResult HitResult;
	if (LineTrace(HitResult))
	{
		FPointDamageEvent DamangeEvent;
		DamangeEvent.HitInfo = HitResult;

		HitResult.GetActor()->TakeDamage(Damage, DamangeEvent, PlayerController, this);
	}
	SpawnBulletTracer();
}

AActor* AHitScanWeapon::SpawnBulletTracer()
{
	UE_LOG(LogTemp, Log, TEXT("AHitScanWeapon::SpawnBulletTracer()"));
	if (BulletTracerBlueprint == NULL) return NULL;
	const FVector SpawnPosition = Muzzle->GetComponentToWorld().GetLocation();
	FMinimalViewInfo CameraViewInfo;
	Cast<AFPSCharacter>(GetOwner())->GetCameraComponent()->GetCameraView(1.0f, CameraViewInfo);
	return GetWorld()->SpawnActor<AActor>(BulletTracerBlueprint->GeneratedClass, SpawnPosition, CameraViewInfo.Rotation);
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
	/*DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		5.f,
		0.f,
		1.f
	);*/

	bool IsHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		PlayerViewPointLocation,
		LineTraceEnd,
		ECollisionChannel::ECC_Pawn,
		LineTraceCollisionQueryParams
	);

	/*if (!IsHit)
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
	}*/

	return IsHit;
}
