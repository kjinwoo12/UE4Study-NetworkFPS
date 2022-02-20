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
			Properties
	***************************/
public:
	UPROPERTY(EditDefaultsOnly, Category = "properties")
	float ExplosionTime;

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
			  Actions
	***************************/
	virtual void StartAction() override;
	virtual void StopAction() override;

	void PlayHandsAnimation();
	void StopHandsAnimation();

	void Plant();

	/**************************
				RPC
	***************************/
};
