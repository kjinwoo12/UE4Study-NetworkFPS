// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WaitingPlayersMode.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AWaitingPlayersMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGameModeBase> PreparedGameModeSubclass;

public:
	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer);

	/**************************
			  OnEvent
	***************************/
	void OnPlayerJoinTeam();
	
	void OnPlayerFull();

	/**************************
				etc
	***************************/
	bool IsPlayerFullOnTeam();
};
