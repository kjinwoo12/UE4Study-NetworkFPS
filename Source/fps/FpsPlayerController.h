// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FpsPlayerController.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPlayerTeam : uint8
{
	TeamTerror UMETA(DisplayName = "Team Terror"),
	TeamCounterTerror UMETA(DisplayName = "Team Counter-Terror")
};

UCLASS()
class FPS_API AFpsPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(Replicated)
	EPlayerTeam Team;

public:
	AFpsPlayerController();

	UFUNCTION(BlueprintCallable)
	void ConnectTo(const FString URL);

	virtual void OnLogin();

	UFUNCTION(Client, Reliable)
	void ClientRPCOnLogin();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void ServerRPCUpdateName(const FString& name);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void ServerRPCUpdateTeam(EPlayerTeam PlayerTeam);
};
