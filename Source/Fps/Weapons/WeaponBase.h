 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hands.h"
#include "../Interface/WeaponEvent.h"
#include "Components/TimeLineComponent.h"
#include "WeaponBase.generated.h"

class AFpsCharacter;
class APickupableActor;
class AHandsModelForBody;

UCLASS()
class FPS_API AWeaponBase : public AHands
{
	GENERATED_BODY()
	
protected:

	/**************************
			 Properties
	***************************/
	// Delay of Action for next action
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float ActionDelay;

	// Delay of Subaction for next action
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float SubactionDelay;
	
	// If it's true, action will be run until StopAction is called.
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	bool ActionLoopEnable;

	// If it's true, subaction will be run until StopSubaction is called.
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	bool SubactionLoopEnable;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float ReloadDelay;

	// Ammo count you can hold once at one magazine
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	int MagazineSize;

	// Left ammo you can use before reload
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Properties")
	int CurrentAmmo;

	// Ammo count for reload
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Properties")
	int SubAmmo;

	// If it's true, you don't consume your ammo.
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	bool IsAmmoInfinite;

	// 1.f ~ 3.f
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float Accuracy;

	// higher is more stable.
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float MovementStability;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float RecoilRecoveryTime;

	float CurrentRecoilRecoveryTime;

	/**************************
			 Animation
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
			  Events
	***************************/
	TArray<IWeaponEvent*> EventObservers;

	/**************************
			  Recoil
	***************************/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", Meta = (AllowPrivateAccess = "true"))
	UCurveVector* CameraRecoilControlCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", Meta = (AllowPrivateAccess = "true"))
	UCurveVector* BulletRecoilCurve;

	FTimeline RecoilControlTimeline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", Meta = (AllowPrivateAccess = "true"))
	UCurveVector* CameraRecoilStabilityCurve;

	FTimeline RecoilStabilityTimeline;

	bool IsOnAutomaticRecoil;

	FVector MaximumCameraRecoilControl;
	FVector CurrentCameraRecoilControl;

	FVector MaximumBulletRecoil;
	FVector CurrentBulletRecoil;

	float MaximumHoldTime;
	float CurrentHoldTime;
	
	/**************************
			    ETC
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

	virtual void Initialize(AFpsCharacter* FpsCharacter) override;

	void InitializeRecoilTimeline();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**************************
			  Recoil
	***************************/
	virtual void RecoilControlTick(float DeltaTime);

	virtual void AutomaticRecoilControlTick(float DeltaTime);

	virtual void RecoilStabilityTick(float DeltaTime);

public:
	/**************************
			  Events
	***************************/
	//WeaponBase
	void AddObserver(IWeaponEvent* Observer);
	void RemoveObserver(IWeaponEvent* Observer);

	//FpsCharacterEvent
	virtual void OnUnequipHands(AHands* Hands) override;
	virtual void OnActionPressed() override;
	virtual void OnActionReleased() override;
	virtual void OnSubactionPressed() override;
	virtual void OnSubactionReleased() override;
	virtual void OnReloadPressed() override;

	//WeaponEvent
	

	//Timeline Callback
	UFUNCTION()
	virtual void OnCameraRecoilControlProgress(FVector CameraRecoil);
	
	UFUNCTION()
	virtual void OnBulletRecoilProgress(FVector BulletRecoil);

	UFUNCTION()
	virtual void OnRecoilControlTimelineFinish();

	UFUNCTION()
	virtual void OnCameraRecoilStabilityProgress(FVector CameraRecoil);

	/**************************
			   Action 
	***************************/
	UFUNCTION(Server, Reliable)
	void ServerRpcStartAction();
	
	UFUNCTION(Server, Reliable)
	void ServerRpcStopAction();

	UFUNCTION(Server, Reliable)
	void ServerRpcStartSubaction();

	UFUNCTION(Server, Reliable)
	void ServerRpcStopSubaction();
	
	UFUNCTION(Server, Reliable)
	void ServerRpcStartReload();

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

	UFUNCTION(BlueprintCallable, Category = "Setter")
	void SetBodyAnimInstance(UAnimInstance* AnimInstance);
};