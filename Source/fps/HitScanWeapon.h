// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "HitScanWeapon.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AHitScanWeapon : public AWeaponBase
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
	// CollisionParams for LineTrace
	FCollisionQueryParams LineTraceCollisionQueryParams;

	FTimerHandle MuzzleEffectTimer;

public:
	// Sets default values for this actor's properties
	AHitScanWeapon();

public:
	virtual void Initialize(AFPSCharacter* FPSCharacter) override;

	/**************************
			 on Events
	***************************/
	virtual void OnUnEquipped() override;

	virtual void OnAction() override;

	UFUNCTION(Client, UnReliable)
	void ClientRPCOnActionFx();

	/**************************
				etc
	***************************/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	AActor* SpawnBulletTracer(const FVector SpawnPosition, const FVector ImpactPoint);

protected:
	// For hit character
	bool LineTrace(FHitResult& HitResult);
};
