// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponEvent.generated.h"

class AWeaponBase;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponEvent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPS_API IWeaponEvent
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnActionEvent(AWeaponBase* WeaponBase);
	virtual void OnSubactionEvent(AWeaponBase* WeaponBase);
	virtual void OnReloadEvent(AWeaponBase* WeaponBase);
	virtual void OnCameraRecoilControlProgress(FVector CameraRecoil);
	virtual void OnCameraRecoilStabilityProgress(FVector CameraRecoil);
	virtual void OnBulletRecoilProgress(FVector BulletRecoil);
	virtual void OnRecoilStop(float RecoveryTime);
};
