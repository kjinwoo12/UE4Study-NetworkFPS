// Fill out your copyright notice in the Description page of Project Settings.


#include "FpsPlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "FpsGameInstance.h"
#include "FpsPlayerState.h"
#include "FpsCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "WaitingPlayersMode.h"

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

	FpsCharacter->OnPossessed();
	ClientSetHUD(FpsCharacter->GetHudSubclass());
}

void AFpsPlayerController::OnPlayerFull()
{
	AFpsCharacter* FpsCharacter = Cast<AFpsCharacter>(GetPawn());
	if (IsValid(FpsCharacter))
	{
		FpsCharacter->OnPlayerFull();
	}
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

bool AFpsPlayerController::ServerRpcOnSelectedTeam_Validate(EPlayerTeam team, TSubclassOf<AFpsCharacter> CharacterClass, FTransform SpawnTransform)
{
	return true;
}

void AFpsPlayerController::ServerRpcOnSelectedTeam_Implementation(EPlayerTeam Team, TSubclassOf<AFpsCharacter> CharacterClass, FTransform SpawnTransform)
{
	UE_LOG(LogTemp, Log, TEXT("AFpsPlayerController::ServerRPCOnSelectedTeam"));
	SetTeam(Team);
	SpawnAsPlayableCharacter(CharacterClass, SpawnTransform);
	
	AWaitingPlayersMode* GameMode = Cast<AWaitingPlayersMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(GameMode)) return;
	GameMode->OnPlayerJoinTeam();
}

void AFpsPlayerController::SetTeam(const EPlayerTeam Team)
{
	AFpsPlayerState* playerState = (AFpsPlayerState*)this->PlayerState;
	playerState->Team = Team;
}

void AFpsPlayerController::SpawnAsPlayableCharacter(TSubclassOf<AFpsCharacter> CharacterClass, FTransform SpawnTransform)
{
	FActorSpawnParameters SpawnParameters;
	AFpsCharacter *SpawnedCharacter = GetWorld()->SpawnActor<AFpsCharacter>(CharacterClass, SpawnTransform.GetLocation(), SpawnTransform.GetRotation().Rotator(), SpawnParameters);
	SpawnedCharacter->SetSpawnTransform(SpawnTransform);
	Possess(SpawnedCharacter);
}