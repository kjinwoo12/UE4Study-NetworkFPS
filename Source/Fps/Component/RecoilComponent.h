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

	FVector RecoilControl;
	FVector RecoilStability;

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

	// IWeaponEvent
	virtual void OnCameraRecoilControlProgress(FVector CameraRecoil) override;
	virtual void OnCameraRecoilStabilityProgress(FVector CameraRecoil) override;
	virtual void OnRecoilStop(float RecoveryTime) override;
};
