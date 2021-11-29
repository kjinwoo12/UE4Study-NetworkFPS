// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpWeapon.generated.h"

class AWeaponBase;

UCLASS()
class FPS_API APickUpWeapon : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* PickUpRange;

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

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex
	);

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
