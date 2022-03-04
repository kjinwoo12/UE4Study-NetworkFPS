// Fill out your copyright notice in the Description page of Project Settings.


#include "Hands.h"
#include "HandsModelForBody.h"
#include "PickupableActor.h"

// Sets default values
AHands::AHands()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	// Properties
	AttachingGripPointName = "GripPoint";
	HandsIndex = 0;

	// Components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	HandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandsMeshs"));
	HandsMesh->SetOnlyOwnerSee(true);
	HandsMesh->SetRelativeLocation(DefaultLocationOfHandsMeshComponent);
	HandsMesh->SetupAttachment(RootComponent);
}

void AHands::Initialize(AActor* Parent)
{
	SetOwner(Parent);
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

void AHands::OnUnEquipped()
{
	UE_LOG(LogTemp, Log, TEXT("OnUnEquipped"));
	ClientRpcOnUnEquipped();
	SetOwner(nullptr);
}

void AHands::StartAction()
{

}

void AHands::StopAction()
{

}

void AHands::StartSubaction()
{

}

void AHands::StopSubaction()
{

}

void AHands::StartReload()
{

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

void AHands::ClientRpcOnUnEquipped_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("ClientRpcOnUnEquipped"));
	SetOwner(nullptr);
}

int AHands::GetHandsIndex()
{
	return HandsIndex;
}

FName AHands::GetAttachingGripPointName()
{
	return AttachingGripPointName;
}