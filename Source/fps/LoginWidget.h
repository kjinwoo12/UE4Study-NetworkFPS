// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Internationalization/Regex.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	bool IsIpExpression(FString str);
};
