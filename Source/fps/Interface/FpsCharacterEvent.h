// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FpsCharacterEvent.generated.h"

class AHands;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFpsCharacterEvent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPS_API IFpsCharacterEvent
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnEquipHands(AHands* Hands);
	virtual void OnUnequipHands(AHands* Hands);

	/**************************
			   Action
	***************************/
	virtual void OnActionPressed();
	virtual void OnActionReleased();
	virtual void OnSubactionPressed();
	virtual void OnSubactionReleased();
	virtual void OnReloadPressed();
};
