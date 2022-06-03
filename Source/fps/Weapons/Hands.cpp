// Fill out your copyright notice in the Description page of Project Settings.


#include "Hands.h"
#include "HandsModelForBody.h"
#include "PickupableActor.h"
#include "../Actors/FpsCharacter.h"

// Sets default values
AHands::AHands()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	// Properties
	HandsIndex = 0;

	// Components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	HandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandsMeshs"));
	HandsMesh->SetOnlyOwnerSee(true);
	HandsMesh->SetRelativeLocation(DefaultLocationOfHandsMeshComponent);
	HandsMesh->SetupAttachment(RootComponent);
}

void AHands::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHands::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHands::Initialize(AFpsCharacter* FpsCharacter)
{
	SetOwner(FpsCharacter);
	FpsCharacter->AddObserver(this);
	SetActorScale3D(FVector(0.2f));
}

void AHands::OnUnequipHands(AHands* Hands)
{
	UE_LOG(LogTemp, Log, TEXT("OnUnequipHands"));
	ClientRpcOnUnequipped();
	SetOwner(nullptr);
}

AHandsModelForBody* AHands::CreateHandsModelForBody()
{
	return GetWorld()->SpawnActor<AHandsModelForBody>(ModelForBodySubclass, FVector(0, 0, 0), FRotator::ZeroRotator);
}

APickupableActor* AHands::CreatePickupableActor()
{
	FRotator Rotation = GetActorRotation();
	return GetWorld()->SpawnActor<APickupableActor>(PickupableActorSubclass, GetActorLocation(), FRotator(90, Rotation.Yaw, 0));
}

void AHands::ClientRpcOnUnequipped_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("ClientRpcOnUnEquipped"));
	SetOwner(nullptr);
}

int AHands::GetHandsIndex()
{
	return HandsIndex;
}