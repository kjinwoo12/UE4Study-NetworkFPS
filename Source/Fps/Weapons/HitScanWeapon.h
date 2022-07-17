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
	
protected:
	/**************************
			 Properties
	***************************/
	// Reach of weapon
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float Reach;

	/**************************
			   Decal
	***************************/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UMaterial* DecalMaterial;

	/**************************
				etc
	***************************/
	// CollisionParams for LineTrace
	FCollisionQueryParams LineTraceCollisionQueryParams;

	FVector PlayerViewPointLocation;

	FRotator PlayerViewPointRotation;

public:
	// Sets default values for this actor's properties
	AHitScanWeapon();

public:
	virtual void Initialize(AFpsCharacter* FpsCharacter) override;

	/**************************
			 on Events
	***************************/
	virtual void OnUnequipHands(AHands* Hands) override;

	virtual void OnAction() override;

	virtual void OnHitResult(FHitResult HitResult, APlayerController* PlayerController);

	/**************************
			  Common
	***************************/
	void GiveDamage(FHitResult HitResult, APlayerController* PlayerController);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRpcSpawnDecalActor(FVector Location, FRotator Rotator);

protected:
	// For hit character
	bool LineTrace(FHitResult& HitResult);
	
	void UpdatePlayerViewPoint(APlayerController* PlayerController);

	void UpdateRecoilOffset();

	/**************************
		  Getter & Setter
	***************************/
	float GetRecoilOffset();
};
