// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerTeam.h"
#include "FpsPlayerState.generated.h"

UCLASS()
class FPS_API AFpsPlayerState : public APlayerState
{
	GENERATED_BODY()
private:
	FString Name;

public:
	UPROPERTY(BlueprintReadWrite, Replicated)
	EPlayerTeam Team;

public:
	AFpsPlayerState();
	virtual void SetPlayerName(const FString& S);
	virtual FString GetPlayerNameCustom() const;
};
