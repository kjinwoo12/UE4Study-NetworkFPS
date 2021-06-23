// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"
#include "DrawDebugHelpers.h"
#include "FPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"

AHitScanWeapon::AHitScanWeapon()
{
	//Components
	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(RootComponent);
	MuzzleFlashLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("MuzzleFlashLight"));
	MuzzleFlashLight->SetIntensity(20000.f);
	MuzzleFlashLight->SetVisibility(false);
	MuzzleFlashLight->SetupAttachment(RootComponent);
	MuzzleFlashTextureEffect = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MuzzleFlashTextureEffect"));
	MuzzleFlashTextureEffect->SetVisibility(false);
	MuzzleFlashTextureEffect->SetupAttachment(RootComponent);
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

	SpawnBulletTracer(Muzzle->GetComponentToWorld().GetLocation(), HitResult.ImpactPoint);
	ClientRPCOnActionFx();
}

void AHitScanWeapon::ClientRPCOnActionFx_Implementation()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	MuzzleFlashLight->SetVisibility(true);
	MuzzleFlashTextureEffect->SetVisibility(true);
	FRotator Rotator = FRotator(FMath::RandRange(-90.f, 90.f), 0, -90);
	MuzzleFlashTextureEffect->SetRelativeRotation(Rotator);
	const float WaitTime = 0.05f;
	TimerManager.ClearTimer(MuzzleEffectTimer);
	TimerManager.SetTimer(
		MuzzleEffectTimer,
		FTimerDelegate::CreateLambda([&]()
		{
			MuzzleFlashLight->SetVisibility(false);
			MuzzleFlashTextureEffect->SetVisibility(false);
		}), 
		WaitTime,
		false);
}

AActor* AHitScanWeapon::SpawnBulletTracer(const FVector SpawnPosition, const FVector ImpactPoint)
{
	UE_LOG(LogTemp, Log, TEXT("AHitScanWeapon::SpawnBulletTracer()"));
	if (BulletTracerBlueprint == NULL) return NULL;
	const FVector BulletDirection = ImpactPoint - SpawnPosition;
	return GetWorld()->SpawnActor<AActor>(BulletTracerBlueprint->GeneratedClass, SpawnPosition, BulletDirection.Rotation());
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

	if (!IsHit)
	{
		HitResult.ImpactPoint = LineTraceEnd;
	}
	
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

	return IsHit;
}
