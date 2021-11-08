// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerTeam.h"
#include "FpsPlayerController.generated.h"

UCLASS()
class FPS_API AFpsPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFpsPlayerController();

	UFUNCTION(BlueprintCallable)
	void ConnectTo(const FString URL);

	virtual void OnLogin();

	UFUNCTION(BlueprintCallable)
	void OnSelectedTeam(EPlayerTeam team, TSubclassOf<class AFpsCharacter> CharacterClass, FTransform SpawnTransform);

	UFUNCTION(BlueprintCallable)
	void OnGameReady();

	/* =========== RPCs============ */
	UFUNCTION(Client, Reliable)
	void ClientRPCOnLogin();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void ServerRPCSetName(const FString& Name);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCSetTeam(EPlayerTeam Team);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCSpawnAsPlayableCharacter(TSubclassOf<class AFpsCharacter> CharacterClass, FTransform SpawnTransform);
};
