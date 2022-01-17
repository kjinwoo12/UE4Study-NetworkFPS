// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveActor.h"
#include "TimeBomb.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API ATimeBomb : public AInteractiveActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* BombMeshComponent;

	UPROPERTY(EditAnywhere)
	float MaxTime = 45.f;

	float CurrentTime;

	bool IsDismantiling = false;

	float MaxDiffusingTime = 7.f;

	float CurrentDiffusingTime = 0.f;

public:
	// Sets default values for this actor's properties
	ATimeBomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	/**************************
		 Called every frame
	***************************/
	virtual void Tick(float DeltaTime) override;

	/**************************
			  On Events
	***************************/
	virtual void OnTargetedBy(ACharacter* character);

	virtual void OnUntargeted(ACharacter* character);

	virtual void OnInteractWith(ACharacter* character);

	virtual void OnInteractionStop(ACharacter* character);

	/**************************
		  Getter & Setter
	***************************/
	float GetCurrentTime();
};
