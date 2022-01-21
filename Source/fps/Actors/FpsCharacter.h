// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FpsCharacter.generated.h"

#define WEAPON_INDEX_SIZE 5

class AGunShop;
class AInteractiveActor;

UENUM(BlueprintType)
enum class EFpsCharacterStatus : uint8
{
	Alive UMETA(DisplayName = "Alive"), // Controllable all
	Stopped UMETA(DisplayName = "Stopped"), // Controllable only Aim
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
	const FVector DefaultLocationOfHandsMeshComponent = FVector(-25, 15, -150.f);

	const FRotator DefaultRotatorOfHandsMeshComponent = FRotator(-7.f, -15.f, 0.f);

	const FVector DefaultLocationOfBodyMeshComponent = FVector(0.f, 0.f, -94.f);

	const FRotator DefaultRotatorOfBodyMeshComponent = FRotator(0.f, -90.f, 0.f);

	const float InteractionReach = 200.f;

	/**************************
			Components
	***************************/
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* HandsMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* BodyMeshComponent;

	UPROPERTY(EditDefaultsOnly, Replicated, ReplicatedUsing = OnRep_InitializePrimaryWeapon)
	class AWeaponBase* PrimaryWeapon;

	UPROPERTY(EditDefaultsOnly, Replicated)
	class AWeaponModelForBody* WeaponModelForBody;

	UCharacterMovementComponent* MovementComponent;

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

	FTransform SpawnTransform;

	/**************************
				Aim
	***************************/
	UPROPERTY(Replicated)
	float AimPitch;

	UPROPERTY(Replicated)
	float AimYaw;

	/**************************
		   Weapon Switch
	***************************/
	UPROPERTY(Replicated)
	int WeaponOnHandIndex = 0;

	UPROPERTY(Replicated)
	TArray<AWeaponBase*> WeaponInventory;
	 
	/**************************
				etc
	***************************/
	// CollisionParams for LineTrace
	FCollisionQueryParams LineTraceCollisionQueryParams;

	UPROPERTY(Replicated)
	AInteractiveActor* InteractiveTarget;

	UPROPERTY(Replicated)
	AGunShop* GunShop;

public:
	// Sets default values for this character's properties
	AFpsCharacter();
	
	/**************************
			Initialize
	***************************/
private:
	void InitializeCollisionComponent();

	void InitializeMovementComponent();

	void InitializeCamera();

	void InitializeHandsMesh();

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

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRpcSetCameraRotation(FQuat CameraRotation);

	void UpdateAim(float DeltaTime);

	void UpdateActorDirection(float DeltaTime);

	void UpdateCameraRotation();

	void UpdateInteractiveTarget(float DeltaTime);

	/**************************
			Bind keys
	***************************/
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void AddControllerPitchInput(float Value) override;

	void AddControllerYawInput(float Value) override;

	void Jump();

	void CrouchPressed();

	void CrouchReleased();

	void ActionPressed();

	void ActionReleased();

	void SubactionPressed();

	void SubactionReleased();

	void ReloadPressed();

	void DropWeaponPressed();

	void InteractionPressed();

	void InteractionReleased();

	void WeaponSwitchPressed(int Index);
	DECLARE_DELEGATE_OneParam(FWeaponSwitchDelegate, int32);

	void GunShopPressed();

	/**************************
			  OnEvent
	***************************/
	void OnPossessed();

	void OnPlayerFull();

	void OnRoundReady();

	void OnRoundStart();

	void OnRoundEnd();

	/**************************
				Rpc
	***************************/
	UFUNCTION(Client, Reliable)
	void ClientRpcUpdateCameraRotationToServer();

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

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRpcPickUpWeapon(APickUpWeapon* PickUpWeapon);

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
	void ServerRpcWeaponSwitch(int Index);

	/**************************
				OnRep
	***************************/
	UFUNCTION()
	void OnRep_InitializePrimaryWeapon();

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
	void EquipWeapon(AWeaponBase* WeaponBase);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeaponBase* UnEquipWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void DropWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AcquireWeapon(AWeaponBase* WeaponBase);

	UFUNCTION(BlueprintCallable, Category ="Weapon")
	void WeaponSwitch(int Index);

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
	AWeaponBase* GetPrimaryWeapon();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	USkeletalMeshComponent* GetHandsMeshComponent();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	USkeletalMeshComponent* GetBodyMeshComponent();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	float GetAimPtich();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	float GetAimYaw();

	TSubclassOf<AHUD> GetHudSubclass();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	AGunShop* GetGunShop();

	void SetSpawnTransform(FTransform Transform);

	void SetCharacterStatus(EFpsCharacterStatus Status);

	void SetInteractiveTarget(AInteractiveActor* Actor);

	void SetWeaponInstanceAtInventory(AWeaponBase* WeaponInstance, int Index);
};