// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FpsCharacter.generated.h"

class AGunShop;

UENUM(BlueprintType)
enum class EFpsCharacterStatus : uint8
{
	Alive UMETA(DisplayName = "Alive"), // Controllable all
	Stopped UMETA(DisplayName = "Stopped"), // Controllable only Mouse
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

	const FName NamePelvis = FName("pelvis");

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
	
	UPROPERTY(Replicated)
	class APickUpWeapon* PickableWeapon;

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
	EFpsCharacterStatus Status;

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
				etc
	***************************/
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
	void TickCrosshair();

	UFUNCTION(Client, Reliable)
	void ClientRPCUpdateCameraToServer();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCSetCameraRotation(FQuat CameraRotation);

	void UpdateActorDirectionByAim(float DeltaTime);

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

	void PickUpWeaponPressed();

	void DropWeaponPressed();

	void GunShopPressed();

	/**************************
			  OnEvent
	***************************/
	void OnPossessed();

	void OnPlayerFull();

	void OnRoundStart();

	/**************************
				RPC
	***************************/
	UFUNCTION(Server, Reliable)
	void ServerRPCStartAction();

	UFUNCTION(Server, Reliable)
	void ServerRPCStopAction();

	UFUNCTION(Server, Reliable)
	void ServerRPCStartSubaction();

	UFUNCTION(Server, Reliable)
	void ServerRPCStopSubaction();

	UFUNCTION(Server, Reliable)
	void ServerRPCStartReload();

	UFUNCTION(Server, Reliable)
	void ServerRPCPickUpWeapon();

	UFUNCTION(Server, Reliable)
	void ServerRPCDropWeapon();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MulticastRPCSetActorRotation(FRotator Rotator);

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
	void KnockoutBodyMesh();

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
	void PickUpWeapon();

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

	UFUNCTION(BlueprintCallable, Category = "Setter")
	void SetPickableWeapon(APickUpWeapon* Instance);

	void SetSpawnTransform(FTransform Transform);
};