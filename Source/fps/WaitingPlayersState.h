// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "WaitingPlayersState.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AWaitingPlayersState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	int maxTeamSize = 1;
	
public:
	UFUNCTION(BlueprintCallable)
	int GetTerrorTeamSize();

	UFUNCTION(BlueprintCallable)
	int GetCounterTerrorTeamSize();
};
