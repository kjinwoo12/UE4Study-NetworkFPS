// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Actors/InteractiveActor.h"
#include "PickupableActor.generated.h"

class AWeaponBase;

UCLASS()
class FPS_API APickupableActor : public AInteractiveActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<AWeaponBase> WeaponBaseSubclass;

	class AHands* HandsInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundCue* DropSound;
	
public:	
	// Sets default values for this actor's properties
	APickupableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/**************************
			  On Events
	***************************/
	virtual void OnTargetedBy(ACharacter* character) override;

	virtual void OnUntargeted(ACharacter* character) override;

	virtual void OnInteractWith(ACharacter* character) override;

	virtual void OnInteractionStop(ACharacter* character) override;

	UFUNCTION()
	void OnWeaponMeshComponentHit(UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		FVector NormalImpulse,
		const FHitResult& HitResult);

	/**************************
		  Getter & Setter
	***************************/
	UFUNCTION(Category = "Getter")
	UStaticMeshComponent* GetMesh();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	AHands* GetHandsInstance();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	TSubclassOf<AWeaponBase> GetWeaponBaseSubclass();

	UFUNCTION(BlueprintCallable, Category = "Setter")
	void SetHandsInstance(AHands* Instance);
};
