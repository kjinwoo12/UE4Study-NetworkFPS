// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FpsGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AFpsGameMode : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
};
