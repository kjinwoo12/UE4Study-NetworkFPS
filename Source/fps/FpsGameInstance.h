// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FpsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UFpsGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	FString localPlayerName;
public:
	virtual void Init();

	/* Getter && Setter */
	UFUNCTION(BlueprintCallable)
	void SetLocalPlayerName(FString name);

	UFUNCTION(BlueprintCallable)
	FString GetLocalPlayerName();
};
