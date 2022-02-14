// Fill out your copyright notice in the Description page of Project Settings.


#include "HandedTimeBomb.h"
#include "../Actors/TimeBomb.h"


AHandedTimeBomb::AHandedTimeBomb()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	
}

void AHandedTimeBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentPlantingTime < PlantingTimer)
	{
		CurrentPlantingTime += DeltaTime;
		return;
	}
	else
	{
		Plant();
	}
}

void AHandedTimeBomb::StartAction()
{
	SetActorTickEnabled(true);
}

void AHandedTimeBomb::StopAction()
{
	SetActorTickEnabled(false);
}

void AHandedTimeBomb::StartSubaction()
{

}

void AHandedTimeBomb::StopSubaction()
{

}

void AHandedTimeBomb::StartReload()
{

}

void AHandedTimeBomb::Plant()
{

}