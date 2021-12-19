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
	static const int MaxTeamSize;
	static const float TransitionTimeForOnRoundReady;
	static const float TransitionTimeForOnRoundStart;
	static const float MaximumTimeForRound;

	UPROPERTY(BlueprintReadOnly, Replicated)
	float RoundProgressTime = 0;
	
public:
	UFUNCTION(BlueprintCallable)
	int GetTerrorTeamSize();

	UFUNCTION(BlueprintCallable)
	int GetCounterTerrorTeamSize();

	UFUNCTION(BlueprintCallable)
	int GetMaxTeamSize();

	UFUNCTION(Blueprintcallable)
	float GetMaximumTimeForRound();
};