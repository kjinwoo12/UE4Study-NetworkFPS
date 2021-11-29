// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingPlayersState.h"
#include "../PlayerController/FpsPlayerState.h"

int AWaitingPlayersState::GetTerrorTeamSize()
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

int AWaitingPlayersState::GetCounterTerrorTeamSize()
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