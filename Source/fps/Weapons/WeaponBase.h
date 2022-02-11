 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hands.h"
#include "WeaponBase.generated.h"

class AFpsCharacter;
class APickupableActor;
class AHandsModelForBody;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None UMETA(DisplayName = "None"),
	Rifle UMETA(DisplayName = "Rifle"),
	Knife UMETA(DisplayName = "Knife"),
};

UCLASS()
class FPS_API AWeaponBase : public AHands
{
	GENERATED_BODY()
	
protected:

	/**************************
			 Properties
	***************************/
	UPROPERTY(EditDefaultsOnly, Category = "properties")
	EWeaponType WeaponType;

	// Delay of Action for next action
	UPROPERTY(EditDefaultsOnly, Category = "properties")
	float ActionDelay;

	// Delay of Subaction for next action
	UPROPERTY(EditDefaultsOnly, Category = "properties")
	float SubactionDelay;
	
	// If it's true, action will be run until StopAction is called.
	UPROPERTY(EditDefaultsOnly, Category = "properties")
	bool ActionLoopEnable;

	// If it's true, subaction will be run until StopSubaction is called.
	UPROPERTY(EditDefaultsOnly, Category = "properties")
	bool SubactionLoopEnable;

	UPROPERTY(EditDefaultsOnly, Category = "properties")
	float ReloadDelay;

	// Ammo count you can hold once at one magazine
	UPROPERTY(EditDefaultsOnly, Category = "properties")
	int MagazineSize;

	// Left ammo you can use before reload
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "properties")
	int CurrentAmmo;

	// Ammo count for reload
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "properties")
	int SubAmmo;

	// If it's true, you don't consume your ammo.
	UPROPERTY(EditDefaultsOnly, Category = "properties")
	bool IsAmmoInfinite;

	// 0.f ~ 1.f
	UPROPERTY(EditDefaultsOnly, Category = "properties")
	float Accuracy;

	// higher is more stable.
	UPROPERTY(EditDefaultsOnly, Category = "properties")
	float MovementStability;

	UPROPERTY(EditDefaultsOnly, Category = "properties")
	float Damage;

	/**************************
		  About animation
	***************************/
	UAnimInstance* BodyAnimInstance;

	// AnimationMontage to play each time do action
	UPROPERTY(EditDefaultsOnly, Category = "Animation - Hands")
	UAnimMontage* HandsActionAnimation;

	// AnimationMontage to play each time do subaction
	UPROPERTY(EditDefaultsOnly, Category = "Animation - Hands")
	UAnimMontage* HandsSubactionAnimation;

	// AnimationMontage to play each time do reload
	UPROPERTY(EditDefaultsOnly, Category = "Animation - Hands")
	UAnimMontage* HandsReloadAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Animation - Body")
	UAnimMontage* BodyActionAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Animation - Body")
	UAnimMontage* BodySubactionAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Animation - Body")
	UAnimMontage* BodyReloadAnimation;

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
	virtual void Initialize(AActor* Parent) override;

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
		  Getter & Setter
	***************************/
	float GetDelay();

	float GetMovementStability();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	int GetCurrentAmmo();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	int GetSubAmmo();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	EWeaponType GetWeaponType();

	UFUNCTION(BlueprintCallable, Category = "Setter")
	void SetBodyAnimInstance(UAnimInstance* AnimInstance);

	/**************************
				etc
	***************************/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	APickupableActor* SpawnPickUpWeaponActor();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	AHandsModelForBody* SpawnModelForBodyActor();

public:
	static AWeaponBase* SpawnWeapon(UWorld* World, UClass* GeneratedBP); 
};