// Fill out your copyright notice in the Description page of Project Settings.


#include "PlantBombMode.h"
#include "PlantBombState.h"
#include "../PlayerController/FpsPlayerController.h"
#include "../PlayerController/FpsPlayerState.h"
#include "../Actors/Characters/FpsCharacter.h"

void APlantBombMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("APlantBombMode::BeginPlay"));
}

void APlantBombMode::PostLogin(APlayerController* newPlayer)
{
	if (GetNetMode() != ENetMode::NM_DedicatedServer) return;
	UE_LOG(LogTemp, Log, TEXT("APlantBombMode::PostLogin"));
	AFpsPlayerController* playerController = (AFpsPlayerController*)newPlayer;
	playerController->OnLogin();
}

void APlantBombMode::OnPlayerJoinTeam()
{
	if (IsPlayerFullOnTeam())
	{
		OnPlayerFull();
	}
}

void APlantBombMode::OnPlayerFull()
{
	UE_LOG(LogTemp, Log, TEXT("APlantBombMode::OnPlayerFull"));
	APlantBombState* State = GetGameState<APlantBombState>();

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		AFpsPlayerController* FpsPlayerController = Cast<AFpsPlayerController>(Iterator->Get());
		if (IsValid(FpsPlayerController))
		{
			FpsPlayerController->OnPlayerFull();
		}
	}

	GetWorldTimerManager().SetTimer(RoundEventTimer, this, &APlantBombMode::OnRoundReady, TransitionTimeForOnRoundReady, false);
}

void APlantBombMode::OnRoundReady()
{
	GetWorldTimerManager().ClearTimer(RoundEventTimer);
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		AFpsPlayerController* FpsPlayerController = Cast<AFpsPlayerController>(Iterator->Get());
		if (IsValid(FpsPlayerController))
		{
			FpsPlayerController->OnRoundReady();
		}
	}

	GetWorldTimerManager().SetTimer(RoundEventTimer, this, &APlantBombMode::OnRoundStart, TransitionTimeForOnRoundStart, false);
}

void APlantBombMode::OnRoundStart()
{
	GetWorldTimerManager().ClearTimer(RoundEventTimer);
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		AFpsPlayerController* FpsPlayerController = Cast<AFpsPlayerController>(Iterator->Get());
		if (IsValid(FpsPlayerController))
		{
			FpsPlayerController->OnRoundStart();
		}
	}

	GetWorldTimerManager().SetTimer(RoundEventTimer, this, &APlantBombMode::OnRoundEnd, TimeForRoundRunTime, false);
}

void APlantBombMode::OnRoundEnd()
{
	GetWorldTimerManager().ClearTimer(RoundEventTimer);
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		AFpsPlayerController* FpsPlayerController = Cast<AFpsPlayerController>(Iterator->Get());
		if (IsValid(FpsPlayerController))
		{
			FpsPlayerController->OnRoundEnd();
		}
	}

	GetWorldTimerManager().SetTimer(RoundEventTimer, this, &APlantBombMode::OnRoundEnd, TransitionTimeForOnRoundReady, false);
}

bool APlantBombMode::IsPlayerFullOnTeam() 
{
	APlantBombState* State = GetGameState<APlantBombState>();
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