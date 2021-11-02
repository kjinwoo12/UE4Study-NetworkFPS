// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingPlayersState.h"
#include "FpsPlayerState.h"

int AWaitingPlayersState::GetTerrorTeamSize()
{
	int size = 0;
	for (APlayerState* state : PlayerArray) 
	{
		AFpsPlayerState* fpsState = (AFpsPlayerState*)state;
		if (fpsState->Team == EPlayerTeam::TeamTerror)
		{
			size++;
		}
	}
	return size;
}

int AWaitingPlayersState::GetCounterTerrorTeamSize()
{
	int size = 0;
	for (APlayerState* state : PlayerArray)
	{
		AFpsPlayerState* fpsState = (AFpsPlayerState*)state;
		if (fpsState->Team == EPlayerTeam::TeamCounterTerror)
		{
			size++;
		}
	}
	return size;
}