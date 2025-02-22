// Fill out your copyright notice in the Description page of Project Settings.


#include "PlantBombMode.h"
#include "PlantBombState.h"
#include "../PlayerController/FpsPlayerController.h"
#include "../PlayerController/FpsPlayerState.h"
#include "../Actors/FpsCharacter.h"

void APlantBombMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("APlantBombMode::BeginPlay"));
	LeadTeam = EPlayerTeam::None;
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

	//For testing on the viewport
	//OnPlayerFull();
}

void APlantBombMode::OnPlayerFull()
{
	UE_LOG(LogTemp, Log, TEXT("APlantBombMode::OnPlayerFull"));
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		AFpsPlayerController* FpsPlayerController = Cast<AFpsPlayerController>(Iterator->Get());
		if (IsValid(FpsPlayerController))
		{
			FpsPlayerController->OnPlayerFull();
		}
	}

	GetWorldTimerManager().SetTimer(RoundEventTimer, this, &APlantBombMode::OnRoundReady, APlantBombState::TransitionTimeForOnRoundReady, false);
}

void APlantBombMode::OnRoundReady()
{
	UE_LOG(LogTemp, Log, TEXT("APlantBombMode::OnRoundReady"));
	LeadTeam = EPlayerTeam::None;

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		AFpsPlayerController* FpsPlayerController = Cast<AFpsPlayerController>(Iterator->Get());
		if (IsValid(FpsPlayerController))
		{
			FpsPlayerController->OnRoundReady();
		}
	}

	GetWorldTimerManager().SetTimer(RoundEventTimer, this, &APlantBombMode::OnRoundStart, APlantBombState::TransitionTimeForOnRoundStart, false);
}

void APlantBombMode::OnRoundStart()
{
	UE_LOG(LogTemp, Log, TEXT("APlantBombMode::OnRoundStart"));
	LeadTeam = EPlayerTeam::TeamCounterTerror;

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		AFpsPlayerController* FpsPlayerController = Cast<AFpsPlayerController>(Iterator->Get());
		if (IsValid(FpsPlayerController))
		{
			FpsPlayerController->OnRoundStart();
		}
	}

	GetWorldTimerManager().SetTimer(RoundEventTimer, this, &APlantBombMode::OnRoundSecondTick, 1.f, true, 0.f);
}

void APlantBombMode::OnRoundSecondTick() 
{
	APlantBombState* state = GetGameState<APlantBombState>();
	if (!IsValid(state)) return;
	state->RoundProgressTime+=1.f;

	if (state->MaximumTimeForRound <= state->RoundProgressTime)
	{
		GetWorldTimerManager().ClearTimer(RoundEventTimer);
		OnRoundEnd();
	}
}

void APlantBombMode::OnBombPlant(float BombTime)
{
	LeadTeam = EPlayerTeam::TeamTerror;

	APlantBombState* state = GetGameState<APlantBombState>();
	if (!IsValid(state)) return;

	state->RoundProgressTime = BombTime;
}

void APlantBombMode::OnRoundEnd()
{
	UE_LOG(LogTemp, Log, TEXT("APlantBombMode::OnRoundEnd"));

	APlantBombState* state = GetGameState<APlantBombState>();
	if (!IsValid(state)) return;
	if (LeadTeam == EPlayerTeam::TeamTerror)
	{
		state->TerrorTeamPoint++;
	}
	else if (LeadTeam == EPlayerTeam::TeamCounterTerror)
	{
		state->CounterTerrorTeamPoint++;
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		AFpsPlayerController* FpsPlayerController = Cast<AFpsPlayerController>(Iterator->Get());
		if (IsValid(FpsPlayerController))
		{
			FpsPlayerController->OnRoundEnd();
		}
	}

	GetWorldTimerManager().SetTimer(RoundEventTimer, this, &APlantBombMode::OnRoundReady, APlantBombState::TransitionTimeForOnRoundReady, false);
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