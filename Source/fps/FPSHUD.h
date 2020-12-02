// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPSHud.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AFPSHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AFPSHUD();
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere)
	class UTexture2D* CrosshairTexture;
};
