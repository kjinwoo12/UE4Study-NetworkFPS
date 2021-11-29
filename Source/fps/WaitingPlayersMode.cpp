// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingPlayersMode.h"
#include "FpsPlayerController.h"
#include "FpsPlayerState.h"
#include "WaitingPlayersState.h"
#include "FpsCharacter.h"

void AWaitingPlayersMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("AWaitingPlayersMode::BeginPlay"));
}

void AWaitingPlayersMode::PostLogin(APlayerController* newPlayer)
{
	if (GetNetMode() != ENetMode::NM_DedicatedServer) return;
	UE_LOG(LogTemp, Log, TEXT("AWaitingPlayersMode::PostLogin"));
	AFpsPlayerController* playerController = (AFpsPlayerController*)newPlayer;
	playerController->OnLogin();
}

void AWaitingPlayersMode::OnPlayerJoinTeam()
{
	if (IsPlayerFullOnTeam())
	{
		OnPlayerFull();
	}
}

void AWaitingPlayersMode::OnPlayerFull()
{
	UE_LOG(LogTemp, Log, TEXT("AWaitingPlayersMode::OnPlayerFull"));
	AWaitingPlayersState* State = GetGameState<AWaitingPlayersState>();

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		AFpsPlayerController* FpsPlayerController = Cast<AFpsPlayerController>(Iterator->Get());
		if (IsValid(FpsPlayerController))
		{
			FpsPlayerController->OnPlayerFull();
		}
	}
}

bool AWaitingPlayersMode::IsPlayerFullOnTeam() 
{
	AWaitingPlayersState* State = GetGameState<AWaitingPlayersState>();
	if (!IsValid(State)) return false;

	const int MaxPlayerSize = State->MaxTeamSize * 2;
	int CurrentPlayerOnTeamSize = 0;
	for (APlayerState* PlayerState : State->PlayerArray)
	{
		AFpsPlayerState* FpsPlayerState = Cast<AFpsPlayerState>(PlayerState);
		if (FpsPlayerState->Team != EPlayerTeam::None) CurrentPlayerOnTeamSize++;
	}
	if (MaxPlayerSize <= CurrentPlayerOnTeamSize)
	{
		return true;
	}
	else
	{
		return false;
	}
}