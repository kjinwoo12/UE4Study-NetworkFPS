// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FpsPlayerState.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPlayerTeam : uint8
{
	None UMETA(DisplayName = "None"),
	TeamTerror UMETA(DisplayName = "Team Terror"),
	TeamCounterTerror UMETA(DisplayName = "Team Counter-Terror")
};

UCLASS()
class FPS_API AFpsPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	FString Name;

	EPlayerTeam Team;
public:
	AFpsPlayerState();
	virtual void SetPlayerName(const FString& S);
	virtual FString GetPlayerNameCustom() const;
};
