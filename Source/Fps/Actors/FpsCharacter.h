// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interface/FpsCharacterEvent.h"
#include "FpsCharacter.generated.h"

#define WEAPON_INDEX_SIZE 5

// Include classes
class AHands;
class AHandsModelForBody;
class AGunShop;
class AInteractiveActor;
class URecoilComponent;

UENUM(BlueprintType)
enum class EFpsCharacterStatus : uint8
{
	Alive UMETA(DisplayName = "Alive"), // Controllable all
	Stopped UMETA(DisplayName = "Stopped"), // Controllable only camera
	Freeze UMETA(DisplayName = "Freeze"), // Controllable nothing
	Dead UMETA(DisplayName = "Dead"), // Same with Freeze but the character is dead.
};

UCLASS()
class FPS_API AFpsCharacter : public ACharacter
{
	GENERATED_BODY()

	/**************************
			   const
	***************************/
	const FVector DefaultLocationOfBodyMeshComponent = FVector(0.f, 0.f, -94.f);

	const FRotator DefaultRotatorOfBodyMeshComponent = FRotator(0.f, -90.f, 0.f);

	const float InteractionReach = 200.f;

	/**************************
			Components
	***************************/
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	URecoilComponent* RecoilComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* BodyMeshComponent;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_InitializeHands)
	AHands* Hands;

	UPROPERTY(Replicated)
	AHandsModelForBody* HandsModelForBody;

	UCharacterMovementComponent* MovementComponent;

	USceneComponent* CameraYawJointComponent;

	USceneComponent* CameraPitchJointComponent;

	/**************************
			SubClass
	***************************/
	UPROPERTY(EditAnywhere)
	TSubclassOf<AHUD> HudSubclass;

	/**************************
			Properties
	***************************/
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float MaxHealth;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Properties")
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float MaxArmor;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Properties")
	float Armor;

	/**************************
		  Character Status
	***************************/
	UPROPERTY(Replicated)
	EFpsCharacterStatus CharacterStatus;

	FTimerHandle RespawnTimerHandle;

	FTimerHandle RoundTextTimerHandle;

	FTransform SpawnTransform;

	/**************************
				Aim
	***************************/
	UPROPERTY(Replicated)
	float BodyMeshAimPitch;

	UPROPERTY(Replicated)
	float BodyMeshAimYaw;

	// CollisionParams for LineTrace
	FCollisionQueryParams LineTraceCollisionQueryParams;

	/**************************
		   Weapon Switch
	***************************/
	UPROPERTY(Replicated)
	int CurrentHandsIndex = 0;

	UPROPERTY(Replicated)
	int PreviousHandsIndex = 0;

	UPROPERTY(Replicated)
	TArray<AHands*> Inventory;

	UPROPERTY(Replicated)
	AGunShop* GunShop;
	 
	/**************************
				etc
	***************************/

	UPROPERTY(Replicated)
	AInteractiveActor* InteractiveTarget;

	UPROPERTY(Replicated)
	bool Plantable = false;

	/**************************
		   Event Observer
	***************************/
	TArray<IFpsCharacterEvent*> EventObservers;

public:
	// Sets default values for this character's properties
	AFpsCharacter();
	
	/**************************
			Initialize
	***************************/
private:
	void InitializeCollisionComponent();

	void InitializeMovementComponent();

	void InitializeCameraJointComponent();

	void InitializeRecoilComponent();

	void InitializeCamera();

	void InitializeBodyMesh();

	void InitializeGameplayVariable();

	void InitializeGunShop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	/**************************
			 Replicate
	***************************/
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**************************
		 Called every frame
	***************************/
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void UpdateCrosshair();

	void UpdateBodyMeshAimOffset(float DeltaTime);

	void UpdateCameraRotation();

	void UpdateInteractiveTarget(float DeltaTime);

	/**************************
			Bind keys
	***************************/
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Movement
	void MoveForward(float Value);
	void MoveRight(float Value);
	void AddControllerPitchInput(float Value) override;
	void AddControllerYawInput(float Value) override;
	void Jump();
	void CrouchPressed();
	void CrouchReleased();

	// Hands
	void ActionPressed();
	void ActionReleased();
	void SubactionPressed();
	void SubactionReleased();
	void ReloadPressed();

	// Interaction
	void DropWeaponPressed();
	void InteractionPressed();
	void InteractionReleased();

	// Switch weapons
	DECLARE_DELEGATE_OneParam(FHandsSwitchDelegate, int32);
	void SwapHandsPressed(int Index);

	// UI
	void GunShopPressed();

	/**************************
			  Event
	***************************/
	void AddObserver(IFpsCharacterEvent* EventObserver);

	void RemoveObserver(IFpsCharacterEvent* EventObserver);

	void OnPossessed();

	void OnPlayerFull();

	void OnRoundReady();

	void OnRoundStart();

	void OnTextDurationFinished();

	void OnRoundEnd();

	/**************************
				Rpc
	***************************/
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRpcSetCameraRotation(FRotator CameraRotation);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRpcPickUp(APickupableActor* PickupableActor);

	UFUNCTION(Server, Reliable)
	void ServerRpcDropWeapon();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MulticastRpcSetActorRotation(FRotator Rotator);

	UFUNCTION(Client, Reliable, WithValidation)
	void ClientRpcSetAlertTextOnHud(const FString& Text);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRpcKnockOutBodyMesh();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRpcSetInteractiveTarget(AInteractiveActor *Actor);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRpcSwapHandsTo(int Index);

	/**************************
				OnRep
	***************************/
	UFUNCTION()
	void OnRep_InitializeHands();

	/**************************
	   For character's combat
	***************************/
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "Character")
	void Die();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void Respawn();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void KnockOutBodyMesh();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void WakeUpBodyMesh();

	/**************************
		   About weapon
	***************************/
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Equip(AHands* HandsInstance);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AHands* Unequip();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void DropWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Acquire(AHands* HandsInstance, int HandsIndex);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SwapHandsTo(int Index);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SwapHandsToPrevious();

	/**************************
			About UI
	***************************/
	void SetAlertTextOnHud(FString Text);

	/**************************
		  Getter & Setter
	***************************/
	UFUNCTION(BlueprintCallable, Category = "Getter")
	float GetHealth();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	float GetArmor();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	UCameraComponent* GetCameraComponent();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	AHands* GetHands();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	USkeletalMeshComponent* GetBodyMeshComponent();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	float GetAimPtich();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	float GetAimYaw();

	TSubclassOf<AHUD> GetHudSubclass();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	AGunShop* GetGunShop();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	AHandsModelForBody* GetHandsModelForBody();

	void SetSpawnTransform(FTransform Transform);

	void SetCharacterStatus(EFpsCharacterStatus Status);

	void SetInteractiveTarget(AInteractiveActor* Actor);

	void SetHandsAtInventory(AHands* HandsInstance, int Index);

	void EnablePlanting(bool IsPlantable);

	bool IsPlantable();
};