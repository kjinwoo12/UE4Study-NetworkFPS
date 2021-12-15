// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerTeam.h"
#include "FpsPlayerController.generated.h"

class AFpsCharacter;

UCLASS()
class FPS_API AFpsPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFpsPlayerController();

	UFUNCTION(BlueprintCallable)
	void ConnectTo(const FString URL);
	/**************************
			 On Event
	***************************/
	virtual void OnLogin();

	virtual void OnPossess(APawn* InPawn);

	void OnPlayerFull();

	void OnRoundReady();

	/**************************
				RPC
	***************************/
	UFUNCTION(Client, Reliable)
	void ClientRPCOnLogin();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void ServerRPCSetName(const FString& Name);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void ServerRpcOnSelectedTeam(EPlayerTeam Team, TSubclassOf<AFpsCharacter> CharacterClass, FTransform SpawnTransform);

	/**************************
				etc
	***************************/
	void SetTeam(EPlayerTeam Team);

	void SpawnAsPlayableCharacter(TSubclassOf<AFpsCharacter> CharacterClass, FTransform SpawnTransform);
};
