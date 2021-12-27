// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveActor.generated.h"

UCLASS()
class FPS_API AInteractiveActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AInteractiveActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**************************
			  On Events
	***************************/
	virtual void OnTartgetedBy(AActor* actor);

	virtual void OnUntargeted();

	virtual void OnInteractWith(AActor* actor);

	virtual void OnInteractionStop(AActor* actor);
};
