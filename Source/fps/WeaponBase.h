// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponInputInterface.h"
#include "WeaponBase.generated.h"

UCLASS()
class FPS_API AWeaponBase : public AActor, public IWeaponInputInterface
{
	GENERATED_BODY()
	
	/**************************
			Components
	***************************/
	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* Muzzle;


	/**************************
			Variable
	***************************/
	// Reach of weapon
	UPROPERTY(EditDefaultsOnly, Category = Variable)
	float Reach;

	// Delay of Action for next action
	UPROPERTY(EditDefaultsOnly, Category = Variable)
	float ActionDelay;

	// Delay of Subaction for next action
	UPROPERTY(EditDefaultsOnly, Category = Variable)
	float SubactionDelay;
	
	// If it's true, action will be run until StopAction is called.
	UPROPERTY(EditDefaultsOnly, Category = Variable)
	bool ActionLoopEnable;

	// If it's true, subaction will be run until StopSubaction is called.
	UPROPERTY(EditDefaultsOnly, Category = Variable)
	bool SubactionLoopEnable;

	UPROPERTY(EditDefaultsOnly, Category = Variable)
	float ReloadDelay;

	// Ammo count you can hold once at one magazine
	UPROPERTY(EditDefaultsOnly, Category = Variable)
	int MagazineSize;

	// Left ammo you can use before reload
	UPROPERTY(EditDefaultsOnly, Category = Variable)
	int CurrentAmmo;

	// Ammo count for reload
	UPROPERTY(EditDefaultsOnly, Category = Variable)
	int SubAmmo;

	// If it's true, you don't consume your ammo.
	UPROPERTY(EditDefaultsOnly, Category = Variable)
	bool IsAmmoInfinite;

	// 0.f ~ 1.f
	UPROPERTY(EditDefaultsOnly, Category = Variable)
	float Accuracy;

	// higher is more stable.
	UPROPERTY(EditDefaultsOnly, Category = Variable)
	float MovementStability;


	/**************************
		  About animation
	***************************/
	// AnimInstance for play animation from parent component
	UAnimInstance* ParentAnimInstance;

	// AnimationMontage to play each time do action
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	class UAnimMontage* ActionAnimation;

	// AnimationMontage to play each time do subaction
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	class UAnimMontage* SubactionAnimation;

	// AnimationMontage to play each time do reload
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	class UAnimMontage* ReloadAnimation;


	/**************************
			  Sounds
	***************************/
	// Sound to play each time do action
	UPROPERTY(EditAnywhere, Category = Gameplay)
	class USoundBase* ActionSound;

	// Sound to play each time do subaction
	UPROPERTY(EditAnywhere, Category = Gameplay)
	class USoundBase* SubactionSound;

	// Sound to play each time do reload
	UPROPERTY(EditAnywhere, Category = Gameplay)
	class USoundBase* ReloadSound;


	/**************************
				etc
	***************************/
private:
	// TimerHandle to make weapon on delay
	FTimerHandle TimerHandle;

	// Pointer of the function that will be run after delay.
	void (AWeaponBase::* FunctionAfterDelay)();

	// TimerHandle for extra input to run function.
	FTimerHandle TimerHandleForExtraInput;

	// Pointer of the function for TimerHandleForExtraInput
	void (AWeaponBase::* FunctionAfterDelayForExtraInput)();

	// CollisionParams for LineTrace
	FCollisionQueryParams LineTraceCollisionQueryParams;

public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/**************************
		IWeaponInputInterface
	***************************/
	virtual void StartAction() override;

	virtual void StopAction() override;

	virtual void StartSubaction() override;

	virtual void StopSubaction() override;

	virtual void Reload() override;

	/**************************
		For playing animation
	***************************/
	void SetParentAnimInstance(UAnimInstance* AnimInstance);
	void PlayAnimMontage(UAnimMontage* AnimMontage);

	/**************************
		  Getter & Setter
	***************************/
	float GetDelay();

	FCollisionQueryParams* GetLineTraceCollisionQueryParams();

	float GetMovementStability();

	UFUNCTION(BlueprintCallable, Category=Gameplay)
	int GetCurrentAmmo();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	int GetSubAmmo();

protected:
	virtual void OnAction();

	virtual void OnSubaction();

	virtual void OnReload();

	// For hit
	void LineTrace();
};