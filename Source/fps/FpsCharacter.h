// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

UCLASS()
class FPS_API AFPSCharacter : public ACharacter
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

	const FName NameGripPoint = FName("GripPoint");

	/**************************
			Components
	***************************/
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* HandsMeshComponent;

	USkeletalMeshComponent* BodyMeshComponent;

	// PrimaryWeapon will be loaded at BeginPlay();
	UPROPERTY(EditDefaultsOnly, Replicated)
	class AWeaponBase* PrimaryWeapon;

	UPROPERTY(Replicated)
	class APickUpWeapon* PickableWeapon;

	class AFPSHUD* HUD;

	UCharacterMovementComponent* MovementComponent;

	/**************************
			  Variable
	***************************/
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	float MaxHealth;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = Gameplay)
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	float MaxArmor;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = Gameplay)
	float Armor;

	bool bIsDead;

	FTimerHandle RespawnTimerHandle;

public:
	// Sets default values for this character's properties
	AFPSCharacter();
	
	/**************************
		Initialize variable
	***************************/
private:
	void InitializeCollisionComponent();

	void InitializeMovementComponent();

	void InitializeCamera();

	void InitializeHandsMesh();

	void InitializeBodyMesh();

	void InitializeGameplayVariable();

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

	UFUNCTION(Client, UnReliable)
	void ClientRPCTickCrosshair();

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

	/**************************
				RPC
	***************************/
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCStartAction(APlayerController* PlayerController);

	UFUNCTION(Server, Reliable)
	void ServerRPCStopAction();

	UFUNCTION(Server, Reliable)
	void ServerRPCStartSubaction();

	UFUNCTION(Server, Reliable)
	void ServerRPCStopSubaction();

	UFUNCTION(Server, Reliable)
	void ServerRPCStartReload();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCRequestEquipWeaponMulticast(AWeaponBase* WeaponBase);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MulticastRPCEquipWeapon(AWeaponBase* WeaponBase);

	UFUNCTION(Server, Reliable)
	void ServerRPCPickUpWeapon();

	UFUNCTION(Server, Reliable)
	void ServerRPCDropWeapon();

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

	void PickUpWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipTestGun();

	/**************************
		  Getter & Setter
	***************************/
	UFUNCTION(BlueprintCallable, Category = "Getter")
	float GetHealth();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	float GetArmor();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	AWeaponBase* GetPrimaryWeapon();

	UFUNCTION(BlueprintCallable, Category = "Setter")
	void SetPickableWeapon(APickUpWeapon* Instance);
};