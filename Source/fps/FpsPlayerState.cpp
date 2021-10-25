// Fill out your copyright notice in the Description page of Project Settings.


#include "FpsPlayerState.h"
#include "Net/UnrealNetwork.h"

AFpsPlayerState::AFpsPlayerState()
{
	bUseCustomPlayerNames = true;
}

void AFpsPlayerState::SetPlayerName(const FString& S)
{
	Name = S;
}

FString AFpsPlayerState::GetPlayerNameCustom() const
{
	return Name;
}

EPlayerTeam AFpsPlayerState::GetTeam() {
	return Team;
}