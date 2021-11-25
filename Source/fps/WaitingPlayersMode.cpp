// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingPlayersMode.h"
#include "FpsPlayerController.h"
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

	AWaitingPlayersState* State = GetGameState<AWaitingPlayersState>();
	if (State->PlayerArray.Num() >= State->MaxTeamSize * 2)
	{
		OnPlayerFull();
	}
}

void AWaitingPlayersMode::OnPlayerFull()
{
	UE_LOG(LogTemp, Log, TEXT("AWaitingPlayersMode::OnPlayerFull"));
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		AFpsCharacter* Character = Cast<AFpsCharacter>(PlayerController->GetPawn());
		if (IsValid(Character))
		{
			Character->OnPlayerFull();
		}
	}
}

void AWaitingPlayersMode::ServerRPCOnPlayerFull_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("AWaitingPlayersMode::ServerRPCOnPlayerFull"));
}