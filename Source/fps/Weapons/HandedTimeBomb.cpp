// Fill out your copyright notice in the Description page of Project Settings.


#include "HandedTimeBomb.h"
#include "../Actors/TimeBomb.h"
#include "../Actors/FpsCharacter.h"

#include "Net/UnrealNetwork.h"


AHandedTimeBomb::AHandedTimeBomb() : AHands()
{
}

void AHandedTimeBomb::BeginPlay()
{
	Super::BeginPlay();
}

void AHandedTimeBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHandedTimeBomb::StartAction()
{
	UE_LOG(LogTemp, Log, TEXT("AHandedTimeBomb::StartAction"));
	PlayHandsAnimation();
	
	float PlantingTime = PlantingAnimation->GetPlayLength();
	GetWorldTimerManager().SetTimer(PlantingTimer, this, &AHandedTimeBomb::Plant, PlantingTime, false);
}

void AHandedTimeBomb::StopAction()
{
	UE_LOG(LogTemp, Log, TEXT("AHandedTimeBomb::StopAction"));
	StopHandsAnimation();

	GetWorldTimerManager().ClearTimer(PlantingTimer);
}

void AHandedTimeBomb::PlayHandsAnimation()
{
	if (!IsValid(PlantingAnimation))
	{
		UE_LOG(LogTemp, Log, TEXT("AHandedTimeBomb::StartAction Planting Animation is invalid"));
		return;
	}

	UAnimInstance* HandsAnimInstance = HandsMesh->GetAnimInstance();
	if (!IsValid(HandsAnimInstance))
	{
		UE_LOG(LogTemp, Log, TEXT("AHandedTimeBomb::StartAction HandsAnimInstance is invalid"));
		return;
	}
	HandsAnimInstance->Montage_Play(PlantingAnimation);
}

void AHandedTimeBomb::StopHandsAnimation()
{
	UAnimInstance* HandsAnimInstance = HandsMesh->GetAnimInstance();
	if (!IsValid(HandsAnimInstance))
	{
		UE_LOG(LogTemp, Log, TEXT("AHandedTimeBomb::StopHandsAnimation HandsAnimInstance is invalid"));
		return;
	}
	HandsAnimInstance->Montage_Stop(0.f);
}

void AHandedTimeBomb::Plant()
{
	UE_LOG(LogTemp, Log, TEXT("AHandedTimeBomb::Plant"));
	AFpsCharacter* Character = Cast<AFpsCharacter>(GetOwner());
	if (!IsValid(Character))
	{
		UE_LOG(LogTemp, Log, TEXT("AHandedTimeBomb::Plant Character is invalid"));
		return;
	}

	if (!IsValid(TimeBombSubclass))
	{
		UE_LOG(LogTemp, Log, TEXT("AHandedTimeBomb::Plant TimeBombSubclass is invalid"));
		return;
	}

	FActorSpawnParameters SpawnParameters;
	GetWorld()->SpawnActor<ATimeBomb>(TimeBombSubclass, Character->GetActorLocation(), FRotator(0, 180, Character->GetActorRotation().Yaw), SpawnParameters);

	Character->SwapHandsToPrevious();
}