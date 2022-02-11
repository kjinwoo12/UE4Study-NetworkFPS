// Fill out your copyright notice in the Description page of Project Settings.


#include "Hands.h"
#include "HandsModelForBody.h"
#include "PickupableActor.h"

// Sets default values
AHands::AHands()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Properties
	AttachingGripPointName = "GripPoint";
	HandsIndex = 0;

	// Components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	HandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandsMeshs"));
	HandsMesh->SetOnlyOwnerSee(true);
	HandsMesh->SetRelativeLocation(DefaultLocationOfWeaponMeshComponent);
	HandsMesh->SetupAttachment(RootComponent);
}

void AHands::Initialize(AActor* Parent)
{
	SetOwner(Parent);
}

// Called when the game starts or when spawned
void AHands::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHands::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHands::OnActionEvent(const FName ActionName, const EInputEvent KeyEvent)
{
}

AHandsModelForBody* AHands::CreateHandsModelForBody()
{
	return GetWorld()->SpawnActor<AHandsModelForBody>(ModelForBodySubclass, FVector(0, 0, 0), FRotator::ZeroRotator);
}

APickupableActor* AHands::CreatePickupableActor()
{
	FRotator Rotation = GetActorRotation();
	return GetWorld()->SpawnActor<APickupableActor>(PickUpWeaponSubclass, GetActorLocation(), FRotator(90, Rotation.Yaw, 0));
}

int AHands::GetHandsIndex()
{
	return HandsIndex;
}

FName AHands::GetAttachingGripPointName()
{
	return AttachingGripPointName;
}