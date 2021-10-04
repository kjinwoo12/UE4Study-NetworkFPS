// Fill out your copyright notice in the Description page of Project Settings.


#include "FpsPlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "FpsGameInstance.h"
#include "Net/UnrealNetwork.h"

AFpsPlayerController::AFpsPlayerController()
{
	bReplicates = true;
}

void AFpsPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFpsPlayerController, Team);
}

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
}

bool AFpsPlayerController::ServerRPCUpdateTeam_Validate(EPlayerTeam playerTeam)
{
	return true;
}

void AFpsPlayerController::ServerRPCUpdateTeam_Implementation(EPlayerTeam playerTeam)
{
	Team = playerTeam;
}