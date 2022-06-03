// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "../Interface/FpsCharacterEvent.h"
#include "../Interface/WeaponEvent.h"
#include "RecoilComponent.generated.h"

class AFpsCharacter;
class AHands;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API URecoilComponent : public USceneComponent, public IFpsCharacterEvent, public IWeaponEvent
{
	GENERATED_BODY()

	FString RecoilDataContext;
	float CurrentRecoveryTime;
	bool bIsActionPressed;

public:	
	// Sets default values for this component's properties
	URecoilComponent();

	void Initialize(AFpsCharacter* FpsCharacter);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// IFpsCharacterEvent
	virtual void OnEquipHands(AHands* Hands) override;
	virtual void OnUnequipHands(AHands* Hands) override;
	virtual void OnActionPressed() override;
	virtual void OnActionReleased() override;

	// IWeaponEvent
	virtual void OnActionEvent(AWeaponBase* WeaponBase) override;

private:
	// Calculate Recoil
	FRotator CalculateCameraRotation(FRotator Base, FRotator ErrorRange);
};
