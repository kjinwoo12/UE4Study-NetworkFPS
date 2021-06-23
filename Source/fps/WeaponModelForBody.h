// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponModelForBody.generated.h"

UCLASS()
class FPS_API AWeaponModelForBody : public AActor
{
	GENERATED_BODY()
protected:
	/**************************
			Components
	***************************/
	UPROPERTY(EditAnywhere, Category = "Component")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	USceneComponent* Muzzle;
public:	
	// Sets default values for this actor's properties
	AWeaponModelForBody();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
