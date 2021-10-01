// Fill out your copyright notice in the Description page of Project Settings.


#include "FpsPlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "FpsGameInstance.h"
#include "FpsGameInstance.h"

void AFpsPlayerController::ConnectTo(const FString Url)
{
	ClientTravel(Url, ETravelType::TRAVEL_Relative);
}

void AFpsPlayerController::OnLogin()
{
	ClientRPCOnLogin();
}

void AFpsPlayerController::ClientRPCOnLogin_Implementation()
{
	UFpsGameInstance* gameInstance = Cast<UFpsGameInstance>(GetGameInstance());
	ServerRPCUpdateName(gameInstance->GetLocalPlayerName());
}

bool AFpsPlayerController::ServerRPCUpdateName_Validate(const FString& name)
{
	return true;
}

void AFpsPlayerController::ServerRPCUpdateName_Implementation(const FString& name)
{
	SetName(name);
	UE_LOG(LogTemp, Log, TEXT("ServerRPCUpdateName : %s"), *name);
}