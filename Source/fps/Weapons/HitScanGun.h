// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitScanWeapon.h"
#include "HitScanGun.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AHitScanGun : public AHitScanWeapon
{
	GENERATED_BODY()
	
private:
	/**************************
			Components
	***************************/
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* Muzzle;

	UPROPERTY(EditDefaultsOnly)
	class UPointLightComponent* MuzzleFlashLight;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MuzzleFlashTextureEffect;

	/**************************
			  Gameplay
	***************************/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UBlueprint* BulletTracerBlueprint;

	/**************************
				etc
	***************************/
	FTimerHandle MuzzleEffectTimer;

public:
	AHitScanGun();

	/**************************
			 on Events
	***************************/
	virtual void OnAction() override;

	UFUNCTION(Client, UnReliable)
	void ClientRPCOnActionFx();

	/**************************
				etc
	***************************/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	AActor* SpawnBulletTracer(const FVector SpawnPosition, const FVector ImpactPoint);
};
