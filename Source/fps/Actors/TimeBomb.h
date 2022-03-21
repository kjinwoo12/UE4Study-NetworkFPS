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

	bool IsOnDefusing = false;

	float MaxDefusingTime = 7.f;

	float CurrentDefusingTime = 0.f;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ExplosionFx;

	UPROPERTY(EditDefaultsOnly)
	USoundWave* ExplosionSoundFx;

	FTimerHandle ExplosionFxTimer;

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
	virtual void OnTargetedBy(ACharacter* character) override;

	virtual void OnUntargeted(ACharacter* character) override;

	virtual void OnInteractWith(ACharacter* character) override;

	virtual void OnInteractionStop(ACharacter* character) override;

	/**************************
			  Actions
	***************************/
	void Activate();

	void Explode();

	/**************************
				RPC
	***************************/
	UFUNCTION(Server, Reliable)
	void ServerRpcActivate();
};
