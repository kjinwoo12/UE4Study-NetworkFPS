// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingPlayersMode.h"
#include "FpsPlayerController.h"

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

void AWaitingPlayersMode::ServerRPCOnPlayerFull_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("AWaitingPlayersMode::ServerRPCOnPlayerFull"));
	
}