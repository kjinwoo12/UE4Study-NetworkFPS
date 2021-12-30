// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Actors/InteractiveActor.h"
#include "PickUpWeapon.generated.h"

class AWeaponBase;

UCLASS()
class FPS_API APickUpWeapon : public AInteractiveActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<AWeaponBase> WeaponBaseSubclass;

	class AWeaponBase* WeaponInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundCue* DropSound;
	
public:	
	// Sets default values for this actor's properties
	APickUpWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/**************************
			  On Events
	***************************/
	virtual void OnTargetedBy(AActor* actor) override;

	virtual void OnUntargeted(AActor* actor) override;

	virtual void OnInteractWith(AActor* actor) override;

	virtual void OnInteractionStop(AActor* actor) override;

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
	UStaticMeshComponent* GetWeaponMesh();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	AWeaponBase* GetWeaponInstance();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	TSubclassOf<AWeaponBase> GetWeaponBaseSubclass();

	UFUNCTION(BlueprintCallable, Category = "Setter")
	void SetWeaponInstance(AWeaponBase* Instance);
};
