// Fill out your copyright notice in the Description page of Project Settings.


#include "FpsPlayerState.h"
#include "Net/UnrealNetwork.h"

AFpsPlayerState::AFpsPlayerState()
{
	bUseCustomPlayerNames = true;
	Team = EPlayerTeam::None;
}

void AFpsPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFpsPlayerState, Name);
	DOREPLIFETIME(AFpsPlayerState, Team);
}

void AFpsPlayerState::SetPlayerName(const FString& S)
{
	Name = S;
}

FString AFpsPlayerState::GetPlayerNameCustom() const
{
	return Name;
}