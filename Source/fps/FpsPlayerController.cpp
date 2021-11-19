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

void AFpsPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AFpsCharacter* FpsCharacter = Cast<AFpsCharacter>(InPawn);
	if (!IsValid(FpsCharacter)) return;

	FpsCharacter->OnGameReady();
	ClientSetHUD(FpsCharacter->GetHudSubclass());
}

void AFpsPlayerController::OnSelectedTeam(EPlayerTeam team, TSubclassOf<class AFpsCharacter> CharacterClass, FTransform SpawnTransform)
{
	ServerRPCSetTeam(team);
	ServerRPCSpawnAsPlayableCharacter(CharacterClass, SpawnTransform);
}

void AFpsPlayerController::ClientRPCOnLogin_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Client OnLogin() %s"), *GetName());
	UFpsGameInstance* gameInstance = Cast<UFpsGameInstance>(GetGameInstance());
	ServerRPCSetName(gameInstance->GetLocalPlayerName());
}

bool AFpsPlayerController::ServerRPCSetName_Validate(const FString& name)
{
	return true;
}

void AFpsPlayerController::ServerRPCSetName_Implementation(const FString& Name)
{
	AFpsPlayerState* State = GetPlayerState<AFpsPlayerState>();
	State->SetPlayerName(Name);
}

bool AFpsPlayerController::ServerRPCSetTeam_Validate(const EPlayerTeam Team)
{
	return true;
}

void AFpsPlayerController::ServerRPCSetTeam_Implementation(const EPlayerTeam Team)
{
	AFpsPlayerState* playerState = (AFpsPlayerState*)this->PlayerState;
	playerState->Team = Team;
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
	SpawnedCharacter->SetSpawnTransform(SpawnTransform);
	Possess(SpawnedCharacter);
}