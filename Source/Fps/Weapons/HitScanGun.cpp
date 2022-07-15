// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanGun.h"
#include "Components/PointLightComponent.h"

AHitScanGun::AHitScanGun() : AHitScanWeapon()
{
	//Components
	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(HandsMesh);
	Muzzle->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	MuzzleFlashLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("MuzzleFlashLight"));
	MuzzleFlashLight->SetIntensity(20000.f);
	MuzzleFlashLight->SetVisibility(false);
	MuzzleFlashLight->SetupAttachment(Muzzle);
	MuzzleFlashTextureEffect = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MuzzleFlashTextureEffect"));
	MuzzleFlashTextureEffect->SetVisibility(false);
	MuzzleFlashTextureEffect->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MuzzleFlashTextureEffect->SetupAttachment(Muzzle);
}

void AHitScanGun::OnAction()
{
	Super::OnAction();
	if (CurrentAmmo <= 0) return;

	FHitResult HitResult;
	LineTrace(HitResult);
	SpawnBulletTracer(Muzzle->GetComponentToWorld().GetLocation(), HitResult.ImpactPoint);
	ClientRPCOnActionFx();
}

void AHitScanGun::ClientRPCOnActionFx_Implementation()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	MuzzleFlashLight->SetVisibility(true);
	MuzzleFlashTextureEffect->SetVisibility(true);
	MuzzleFlashTextureEffect->SetRelativeRotation(FRotator(0, FMath::RandRange(-90.f, 90.f), 0));
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

AActor* AHitScanGun::SpawnBulletTracer(const FVector SpawnPosition, const FVector ImpactPoint)
{
	UE_LOG(LogTemp, Log, TEXT("AHitScanWeapon::SpawnBulletTracer()"));
	if (BulletTracerBlueprint == NULL) return NULL;
	const FVector BulletDirection = ImpactPoint - SpawnPosition;
	return GetWorld()->SpawnActor<AActor>(BulletTracerBlueprint->GeneratedClass, SpawnPosition, BulletDirection.Rotation());
}