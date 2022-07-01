// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InteractiveActor.generated.h"

UCLASS()
class FPS_API AInteractiveActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AInteractiveActor();

public:	
	/**************************
			  On Events
	***************************/
	virtual void OnTargetedBy(ACharacter* character);

	virtual void OnUntargeted(ACharacter* character);

	virtual void OnInteractWith(ACharacter* character);

	virtual void OnInteractionStop(ACharacter* character);
};
