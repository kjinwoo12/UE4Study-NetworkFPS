 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class AFPSCharacter;

UCLASS()
class FPS_API AWeaponBase : public AActor
{
	GENERATED_BODY()

protected:
	/**************************
			   const
	***************************/

	/**************************
			Components
	***************************/
	UPROPERTY(EditAnywhere, Category = "Component")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* Muzzle;

	/**************************
			  Variable
	***************************/
	// Reach of weapon
	UPROPERTY(EditDefaultsOnly, Category = "Variable")
	float Reach;

	// Delay of Action for next action
	UPROPERTY(EditDefaultsOnly, Category = "Variable")
	float ActionDelay;

	// Delay of Subaction for next action
	UPROPERTY(EditDefaultsOnly, Category = "Variable")
	float SubactionDelay;
	
	// If it's true, action will be run until StopAction is called.
	UPROPERTY(EditDefaultsOnly, Category = "Variable")
	bool ActionLoopEnable;

	// If it's true, subaction will be run until StopSubaction is called.
	UPROPERTY(EditDefaultsOnly, Category = "Variable")
	bool SubactionLoopEnable;

	UPROPERTY(EditDefaultsOnly, Category = "Variable")
	float ReloadDelay;

	// Ammo count you can hold once at one magazine
	UPROPERTY(EditDefaultsOnly, Category = "Variable")
	int MagazineSize;

	// Left ammo you can use before reload
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Variable")
	int CurrentAmmo;

	// Ammo count for reload
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Variable")
	int SubAmmo;

	// If it's true, you don't consume your ammo.
	UPROPERTY(EditDefaultsOnly, Category = "Variable")
	bool IsAmmoInfinite;

	// 0.f ~ 1.f
	UPROPERTY(EditDefaultsOnly, Category = "Variable")
	float Accuracy;

	// higher is more stable.
	UPROPERTY(EditDefaultsOnly, Category = "Variable")
	float MovementStability;

	UPROPERTY(EditDefaultsOnly, Category = "Variable")
	float Damage;


	/**************************
		  About animation
	***************************/
	// AnimInstance for play animation from parent component
	UAnimInstance* ParentAnimInstance;

	// AnimationMontage to play each time do action
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UAnimMontage* ActionAnimation;

	// AnimationMontage to play each time do subaction
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UAnimMontage* SubactionAnimation;

	// AnimationMontage to play each time do reload
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UAnimMontage* ReloadAnimation;

	/**************************
			  Sounds
	***************************/
	// Sound to play each time do action
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase* ActionSound;

	// Sound to play each time do subaction
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase* SubactionSound;

	// Sound to play each time do reload
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase* ReloadSound;

	/**************************
				etc
	***************************/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UBlueprint* PickUpWeaponBlueprint;

	APlayerController* PlayerController;

private:
	// TimerHandle to make weapon on delay
	FTimerHandle TimerHandle;

	// Pointer of the function that will be run after delay.
	void (AWeaponBase::* FunctionAfterDelay)();

	// TimerHandle for extra input to run function.
	FTimerHandle TimerHandleForExtraInput;

	// Pointer of the function for TimerHandleForExtraInput
	void (AWeaponBase::* FunctionAfterDelayForExtraInput)();


public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	/**************************
			 on Events
	***************************/
	virtual void Initialize(AFPSCharacter* FPSCharacter);

	virtual void OnUnEquipped();

	/**************************
		   Weapon Action 
	***************************/
	void StartAction();
	void StopAction();
	void StartSubaction();
	void StopSubaction();
	void StartReload();

	/**************************
			about Actions
	***************************/
	virtual void OnAction();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCOnActionFx();
	
	void OnSubaction();
	
	void OnReload();
	
	/**************************
		For playing animation
	***************************/
	void PlayAnimMontage(UAnimMontage* AnimMontage);

	/**************************
		  Getter & Setter
	***************************/
	float GetDelay();

	float GetMovementStability();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	int GetCurrentAmmo();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	int GetSubAmmo();

	void SetParentAnimInstance(UAnimInstance* AnimInstance);

	APlayerController* GetPlayerController();

	void SetPlayerController(APlayerController* Instance);

	/**************************
				etc
	***************************/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	APickUpWeapon* SpawnPickUpWeaponActor();

public:
	// For Spawn Weapon using Blueprint class path
	// etc 
	static AWeaponBase* SpawnWeapon(UWorld* World, FString WeaponReference);

	static AWeaponBase* SpawnWeapon(UWorld* World, UClass* GeneratedBP);
};