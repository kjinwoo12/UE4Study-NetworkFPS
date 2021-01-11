// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPSHUD.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AFPSHUD : public AHUD
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	float CrosshairDefaultOffset;

	float CrosshairOffset;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	class UTexture2D* CrosshairTexture;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	class UUserWidget* DefaultWidget;
	
public:
	AFPSHUD();
	virtual void DrawHUD() override;
	void SetCrosshairCenterOffset(float Value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void DrawCrosshair();
};
