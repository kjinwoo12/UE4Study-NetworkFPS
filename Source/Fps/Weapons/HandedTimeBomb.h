// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hands.h"
#include "HandedTimeBomb.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AHandedTimeBomb : public AHands
{
	GENERATED_BODY()
	
	/**************************
			 Animation
	***************************/
	// AnimationMontage to play each time do action
	UPROPERTY(EditDefaultsOnly, Category = "Animation - Hands")
	UAnimMontage* PlantingAnimation;

	/**************************
			 Planting
	***************************/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<class ATimeBomb> TimeBombSubclass;

	FTimerHandle PlantingTimer;


public:
	AHandedTimeBomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**************************
			   Event
	***************************/
	virtual void OnActionPressed() override;
	virtual void OnActionReleased() override;

	/**************************
			  Actions
	***************************/
	void StartAction();
	void StopAction();

	void PlayHandsAnimation();
	void StopHandsAnimation();

	void Plant();
};
