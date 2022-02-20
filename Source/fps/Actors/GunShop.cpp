// Fill out your copyright notice in the Description page of Project Settings.

#include "GunShop.h"
#include "../Weapons/Hands.h"
#include "FpsCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
AGunShop::AGunShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("DefaultComponent"));
}

// Called when the game starts or when spawned
void AGunShop::BeginPlay()
{
	Super::BeginPlay();	
}

bool AGunShop::ServerRpcBuyItem_Validate(TSubclassOf<AHands> HandsSubclass)
{
	return true;
}

void AGunShop::ServerRpcBuyItem_Implementation(TSubclassOf<AHands> HandsSubclass)
{
	UE_LOG(LogTemp, Log, TEXT("AGunShop::ServerRpcBuyItem"));
	AFpsCharacter* OwnerCharacter = Cast<AFpsCharacter>(GetOwner());
	if (!IsValid(OwnerCharacter))
	{
		UE_LOG(LogTemp, Log, TEXT("AGunShop::ServerRpcBuyItem / OwnerCharacter is invalid"));
		return;
	}
	APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Log, TEXT("PlayerController is invalid"));
		return;
	}
	AFpsCharacter* FpsCharacter = PlayerController->GetPawn<AFpsCharacter>();
	if (!IsValid(FpsCharacter))
	{
		UE_LOG(LogTemp, Log, TEXT("FpsCharacter is invalid"));
		return;
	}
	AHands* Hands = GetWorld()->SpawnActor<AHands>(HandsSubclass);
	if (!IsValid(Hands))
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to spawn weapon"));
		return;
	}
	FpsCharacter->Acquire(Hands, Hands->GetHandsIndex());
}