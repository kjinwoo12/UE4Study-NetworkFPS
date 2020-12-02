// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSHud.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"


AFPSHUD::AFPSHUD()
{

}

void AFPSHUD::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("We are using FPSHUD."));
	}
}

void AFPSHUD::DrawHUD() {
	Super::DrawHUD();

	FVector2D CrossHairPosition(Canvas->ClipX * 0.5, Canvas->ClipY * 0.5f);
	FCanvasTileItem TileItem(CrossHairPosition, CrosshairTexture->Resource, FColor::Green);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
}