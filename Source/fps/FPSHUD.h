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

	virtual void DrawHUD() override;

	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* CrosshairTexture;
};
