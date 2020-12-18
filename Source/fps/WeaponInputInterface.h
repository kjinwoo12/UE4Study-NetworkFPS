// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"
#include "UObject/Interface.h"
#include "WeaponInputInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponInputInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPS_API IWeaponInputInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void StartAction() = 0;
	virtual void StopAction() = 0;
	virtual void StartSubaction() = 0;
	virtual void StopSubaction() = 0;
	virtual void Reload() = 0;
};
