// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlantBombMode.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API APlantBombMode : public AGameModeBase
{
	GENERATED_BODY()
	
private:
	FTimerHandle RoundEventTimer;
	
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

	void OnRoundReady();

	void OnRoundStart();

	void OnRoundSecondTick();

	void OnRoundEnd();

	/**************************
				etc
	***************************/
	bool IsPlayerFullOnTeam();
};
