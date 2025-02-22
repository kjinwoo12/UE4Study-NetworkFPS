// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FpsCharacterHud.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AFpsCharacterHud : public AHUD
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Variable")
	float CrosshairDefaultOffset;

	float CrosshairOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Texture")
	class UTexture2D* CrosshairTexture;

	UPROPERTY(EditAnywhere,
				BlueprintReadOnly,
				Category = "Widget", 
				Meta=(AllowPrivateAccess=true))
	TSubclassOf<class UUserWidget> DefaultWidgetClass;

	UUserWidget* DefaultWidget;

	UPROPERTY(EditAnywhere,
	BlueprintReadOnly,
		Category = "Widget",
		Meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> GunShopWidgetClass;

	class UGunShopWidget* GunShopWidget;
	
public:
	AFpsCharacterHud();
	virtual void DrawHUD() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void InitDefaultWidget();
	void InitGunShopWidget();
	void DrawCrosshair();

public:
	void OpenGunShop();
	void CloseGunShop();
	bool IsOpenGunShop();

	/**************************
		  Getter & Setter
	***************************/
public:
	UUserWidget* GetDefaultWidget();
	void SetCrosshairCenterOffset(float Value);
};
