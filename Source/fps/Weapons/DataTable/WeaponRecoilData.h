// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponRecoilData.generated.h"

USTRUCT(BlueprintType)
struct FPS_API FWeaponRecoilData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	FVector CameraRecoil;

	UPROPERTY(EditDefaultsOnly)
	FVector CameraRecoilError;

	UPROPERTY(EditDefaultsOnly)
	FVector2D BulletOffset;

	UPROPERTY(EditDefaultsOnly)
	FVector2D BulletOffsetError;

	UPROPERTY(EditDefaultsOnly)
	float RecoilRecoveryTime;
};
