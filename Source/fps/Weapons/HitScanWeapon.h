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
	UPROPERTY(EditDefaultsOnly, Category = "properties")
	float Reach;

	/**************************
				etc
	***************************/
	// CollisionParams for LineTrace
	FCollisionQueryParams LineTraceCollisionQueryParams;
public:
	// Sets default values for this actor's properties
	AHitScanWeapon();

public:
	virtual void Initialize(AActor* Parent) override;

	/**************************
			 on Events
	***************************/
	virtual void OnUnEquipped() override;

	virtual void OnAction() override;

protected:
	// For hit character
	bool LineTrace(FHitResult& HitResult);
};
