// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PlantBombState.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API APlantBombState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	int MaxTeamSize = 1;
	
public:
	UFUNCTION(BlueprintCallable)
	int GetTerrorTeamSize();

	UFUNCTION(BlueprintCallable)
	int GetCounterTerrorTeamSize();
};