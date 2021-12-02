// Fill out your copyright notice in the Description page of Project Settings.


#include "FpsCharacterWidget.h"
#include "Components/TextBlock.h"
void UFpsCharacterWidget::SetAlertText(FString Text)
{
	if (!IsValid(AlertTextBlock))
	{
		UE_LOG(LogTemp, Log, TEXT("UFpsCharacterWidget::SetAlertText / AlertTextBlock is invalid"));
		return;
	}
	AlertTextBlock->SetText(FText::FromString(Text));
}