// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"

AFPSHUD::AFPSHUD()
{
	CrosshairDefaultOffset = 0.f;
	CrosshairOffset = 0.f;
}

AFPSHUD::~AFPSHUD()
{
	if (DefaultWidget) DefaultWidget->RemoveFromViewport();
}

void AFPSHUD::BeginPlay()
{
	Super::BeginPlay();
	if (DefaultWidget) DefaultWidget->AddToViewport();
}

void AFPSHUD::DrawHUD()
{
	Super::DrawHUD();

	if (CrosshairTexture) DrawCrosshair();
}


void AFPSHUD::DrawCrosshair()
{
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	const FVector2D TextureOffset(CrosshairTexture->GetSizeX() * 0.5f, CrosshairTexture->GetSizeY() * 0.5f);
	const float CenterOffset = CrosshairDefaultOffset + CrosshairOffset;
	const FVector2D CrosshairLeftPosition(
		Center.X - TextureOffset.X - CenterOffset,
		Center.Y
	);
	const FVector2D CrosshairTopPosition(
		Center.X - TextureOffset.X,
		Center.Y - CenterOffset
	);
	const FVector2D CrosshairRightPosition(
		Center.X - TextureOffset.X + CenterOffset,
		Center.Y
	);
	const FVector2D CrosshairBottomPosition(
		Center.X - TextureOffset.X,
		Center.Y + CenterOffset
	);

	FCanvasTileItem Left(CrosshairLeftPosition, CrosshairTexture->Resource, FLinearColor::Green);
	Left.BlendMode = SE_BLEND_Translucent;
	Left.PivotPoint.X = 0.5f;
	Left.PivotPoint.Y = 0.f;
	Left.Rotation.Add(0, 90, 0);

	FCanvasTileItem Top(CrosshairTopPosition, CrosshairTexture->Resource, FLinearColor::Green);
	Top.BlendMode = SE_BLEND_Translucent;
	Top.PivotPoint.X = 0.5f;
	Top.PivotPoint.Y = 0.f;
	Top.Rotation.Add(0, 180, 0);

	FCanvasTileItem Right(CrosshairRightPosition, CrosshairTexture->Resource, FLinearColor::Green);
	Right.BlendMode = SE_BLEND_Translucent;
	Right.PivotPoint.X = 0.5f;
	Right.PivotPoint.Y = 0.f;
	Right.Rotation.Add(0, -90, 0);

	FCanvasTileItem Bottom(CrosshairBottomPosition, CrosshairTexture->Resource, FLinearColor::Green);
	Bottom.BlendMode = SE_BLEND_Translucent;

	Canvas->DrawItem(Left);
	Canvas->DrawItem(Top);
	Canvas->DrawItem(Right);
	Canvas->DrawItem(Bottom);
}

void AFPSHUD::SetCrosshairCenterOffset(float Value)
{
	CrosshairOffset = Value;
}