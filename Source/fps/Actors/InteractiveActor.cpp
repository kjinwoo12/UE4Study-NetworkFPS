// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActor.h"
#include "Components/ShapeComponent.h"

// Sets default values
AInteractiveActor::AInteractiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractiveActor::OnTargetedBy(ACharacter* character)
{
	UE_LOG(LogTemp, Log, TEXT("AInteractiveActor::OnTargetedBy"));
}

void AInteractiveActor::OnUntargeted(ACharacter* character)
{
	UE_LOG(LogTemp, Log, TEXT("AInteractiveActor::OnUntargeted"));
}

void AInteractiveActor::OnInteractWith(ACharacter* character)
{
	UE_LOG(LogTemp, Log, TEXT("AInteractiveActor::OnInteractWith"));
}

void AInteractiveActor::OnInteractionStop(ACharacter* character)
{
	UE_LOG(LogTemp, Log, TEXT("AInteractiveActor::OnInteractionStop"));
}