// Fill out your copyright notice in the Description page of Project Settings.


#include "FpsGameInstance.h"

void UFpsGameInstance::Init()
{
	/* Todo : Get last player-name from ini */
	//localPlayerName = getLocalPlayerNameFromIni() - not exist function yet
	localPlayerName = TEXT("DefaultName");
}

void UFpsGameInstance::SetLocalPlayerName(FString name)
{
	/* Todo : Save name at ini*/
	//saveIni(fileName, key, value=name) - not exist function yet
	localPlayerName = name;
}

FString UFpsGameInstance::GetLocalPlayerName()
{
	return localPlayerName;
}