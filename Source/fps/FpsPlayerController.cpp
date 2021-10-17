// Fill out your copyright notice in the Description page of Project Settings.


#include "FpsPlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "FpsGameInstance.h"
#include "FpsPlayerState.h"
#include "FpsCharacter.h"

AFpsPlayerController::AFpsPlayerController()
{
	bReplicates = true;
}

void AFpsPlayerController::ConnectTo(const FString Url)
{
	ClientTravel(Url, ETravelType::TRAVEL_Relative);
}

void AFpsPlayerController::OnLogin()
{
	UE_LOG(LogTemp, Log, TEXT("Server OnLogin() %s"), *GetName());
	ClientRPCOnLogin();
}

void AFpsPlayerController::ClientRPCOnLogin_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Client OnLogin() %s"), *GetName());
	UFpsGameInstance* gameInstance = Cast<UFpsGameInstance>(GetGameInstance());
	ServerRPCUpdateName(gameInstance->GetLocalPlayerName());
}

bool AFpsPlayerController::ServerRPCUpdateName_Validate(const FString& name)
{
	return true;
}

void AFpsPlayerController::ServerRPCUpdateName_Implementation(const FString& Name)
{
	AFpsPlayerState* State = GetPlayerState<AFpsPlayerState>();
	State->SetPlayerName(Name);
}

bool AFpsPlayerController::ServerRPCSpawnAsPlayableCharacter_Validate(TSubclassOf<AFpsCharacter> CharacterClass, FTransform SpawnTransform)
{
	if (CharacterClass) return true;
	else return false;
}

void AFpsPlayerController::ServerRPCSpawnAsPlayableCharacter_Implementation(TSubclassOf<AFpsCharacter> CharacterClass, FTransform SpawnTransform)
{
	FActorSpawnParameters SpawnParameters;
	AFpsCharacter *SpawnedCharacter = GetWorld()->SpawnActor<AFpsCharacter>(CharacterClass, SpawnTransform.GetLocation(), SpawnTransform.GetRotation().Rotator(), SpawnParameters);
	Possess(SpawnedCharacter);
}