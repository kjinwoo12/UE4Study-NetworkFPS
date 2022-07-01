// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FpsCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UFpsCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* AlertTextBlock;

public:
	UFUNCTION(BlueprintCallable)
	void SetAlertText(FString Text);
};
