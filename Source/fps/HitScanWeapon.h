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
	// CollisionParams for LineTrace
	FCollisionQueryParams LineTraceCollisionQueryParams;


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

	/**************************
		   Other Method
	***************************/
public:
protected:
	// For hit character
	bool LineTrace(FHitResult& HitResult);
};
