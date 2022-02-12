// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hands.h"
#include "HandedTimeBomb.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AHandedTimeBomb : public AHands
{
	GENERATED_BODY()
	
	/**************************
			Properties
	***************************/
public:
	UPROPERTY(EditDefaultsOnly, Category = "properties")
	float PlantingTimer;

	float CurrentPlantingTime = 0;

	UPROPERTY(EditDefaultsOnly, Category = "properties")
	float ExplosionTimer;

public:
	/**************************
			  Actions
	***************************/
	virtual void StartAction() override;
	virtual void StopAction() override;
	virtual void StartSubaction() override;
	virtual void StopSubaction() override;
	virtual void StartReload() override;

	void Plant();
};
