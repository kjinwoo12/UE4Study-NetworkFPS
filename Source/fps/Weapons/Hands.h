// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hands.generated.h"

class APickupableActor;
class AHandsModelForBody;

UCLASS()
class FPS_API AHands : public AActor
{
	GENERATED_BODY()

	/**************************
			   const
	***************************/
const FVector DefaultLocationOfWeaponMeshComponent = FVector(0, 0, -150.f);

protected:
	/**************************
			Components
	***************************/
	UPROPERTY(EditAnywhere, Category = "Component")
	USkeletalMeshComponent* HandsMesh;

	/**************************
			Properties
	***************************/
	UPROPERTY(EditDefaultsOnly, Category = "properties")
	FName AttachingGripPointName;

	UPROPERTY(EditDefaultsOnly, Category = "properties")
	int HandsIndex;


	/**************************
			  Gameplay
	***************************/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<APickupableActor> PickUpWeaponSubclass;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<AHandsModelForBody> ModelForBodySubclass;
	
public:	
	// Sets default values for this actor's properties
	AHands();

	/**************************
			Initialize
	***************************/
	virtual void Initialize(AActor* Parent);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**************************
			  On Event
	***************************/
	virtual void OnActionEvent(const FName ActionName, const EInputEvent KeyEvent);

	/**************************
			Game Play
	***************************/
	AHandsModelForBody* CreateHandsModelForBody();

	APickupableActor* CreatePickupableActor();

	/**************************
		  Getter & Setter
	***************************/
	int GetHandsIndex();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	FName GetAttachingGripPointName();
};
