// Fill out your copyright notice in the Description page of Project Settings.


#include "PlantBombState.h"
#include "../PlayerController/FpsPlayerState.h"
#include "Net/UnrealNetwork.h"

const int APlantBombState::MaxTeamSize = 1;
const float APlantBombState::TransitionTimeForOnRoundReady = 2.f;
const float APlantBombState::TransitionTimeForOnRoundStart = 3.f;
const float APlantBombState::MaximumTimeForRound = 160.f;

void APlantBombState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlantBombState, RoundProgressTime);
}

int APlantBombState::GetTerrorTeamSize()
{
	int Size = 0;
	for (APlayerState* State : PlayerArray) 
	{
		AFpsPlayerState* FpsState = (AFpsPlayerState*)State;
		if (FpsState->Team == EPlayerTeam::TeamTerror)
		{
			Size++;
		}
	}
	return Size;
}

int APlantBombState::GetCounterTerrorTeamSize()
{
	int Size = 0;
	for (APlayerState* State : PlayerArray)
	{
		AFpsPlayerState* FpsState = (AFpsPlayerState*)State;
		if (FpsState->Team == EPlayerTeam::TeamCounterTerror)
		{
			Size++;
		}
	}
	return Size;
}

int APlantBombState::GetMaxTeamSize()
{
	return MaxTeamSize;
}

float APlantBombState::GetMaximumTimeForRound()
{
	return MaximumTimeForRound;
}

