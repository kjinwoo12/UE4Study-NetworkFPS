// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/FpsCharacterEvent.h"
#include "Hands.generated.h"

class AFpsCharacter;
class APickupableActor;
class AHandsModelForBody;

UCLASS()
class FPS_API AHands : public AActor, public IFpsCharacterEvent
{
	GENERATED_BODY()

	/**************************
			   const
	***************************/
const FVector DefaultLocationOfHandsMeshComponent = FVector(0, 0, -150.f);

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
	int HandsIndex;

	/**************************
			  Subclass
	***************************/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<APickupableActor> PickupableActorSubclass;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<AHandsModelForBody> ModelForBodySubclass;

public:	
	// Sets default values for this actor's properties
	AHands();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/**************************
			Initialize
	***************************/
	virtual void Initialize(AFpsCharacter* FpsCharacter);

	/**************************
			   Events
	***************************/
	virtual void OnUnequipHands(AHands* Hands) override;

	/**************************
			Game Play
	***************************/
	AHandsModelForBody* CreateHandsModelForBody();

	APickupableActor* CreatePickupableActor();

	/**************************
				RPC
	***************************/
	UFUNCTION(Client, Reliable)
	void ClientRpcOnUnequipped();

	/**************************
		  Getter & Setter
	***************************/
	int GetHandsIndex();

};
