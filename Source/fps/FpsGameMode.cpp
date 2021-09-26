// Fill out your copyright notice in the Description page of Project Settings.


#include "FpsGameMode.h"

void AFpsGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hellow world!"));
	}
}