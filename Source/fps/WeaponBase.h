 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class AFpsCharacter;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None UMETA(DisplayName = "None"),
	Rifle UMETA(DisplayName = "Rifle"),
	Knife UMETA(DisplayName = "Knife"),
};

UCLASS()
class FPS_API AWeaponBase : public AActor
{
	GENERATED_BODY()

protected:

	/**************************
			Components
	***************************/
	UPROPERTY(EditAnywhere, Category = "Component")
	USkeletalMeshComponent* WeaponMesh;

	/**************************
			 Properties
	***************************/
	UPROPERTY(EditDefaultsOnly, Category = "properties")
	FName AttachingGripPointName;

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
	// AnimInstance for play animation from parent component
	UAnimInstance* HandsAnimInstance;

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
			  Gameplay
	***************************/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UBlueprint* PickUpWeaponBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UBlueprint* WeaponModelForBodyBlueprint;

	/**************************
				etc
	***************************/

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
	virtual void Initialize(AFpsCharacter* FPSCharacter);

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
	APlayerController* GetPlayerController();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	FName GetAttachingGripPointName();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	EWeaponType GetWeaponType();

	UFUNCTION(BlueprintCallable, Category = "Setter")
	void SetHandsAnimInstance(UAnimInstance* AnimInstance);

	UFUNCTION(BlueprintCallable, Category = "Setter")
	void SetBodyAnimInstance(UAnimInstance* AnimInstance);

	UFUNCTION(BlueprintCallable, Category = "Setter")
	void SetPlayerController(APlayerController* Instance);

	/**************************
				etc
	***************************/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	APickUpWeapon* SpawnPickUpWeaponActor();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	AWeaponModelForBody* SpawnWeaponModelForBodyActor();

public:
	// For Spawn Weapon using Blueprint class path
	// etc 
	static AWeaponBase* SpawnWeapon(UWorld* World, FString WeaponReference);

	static AWeaponBase* SpawnWeapon(UWorld* World, UClass* GeneratedBP);
};