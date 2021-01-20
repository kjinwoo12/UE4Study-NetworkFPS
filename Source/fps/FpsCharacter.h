// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

UCLASS()
class FPS_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* HandsMeshComponent;
	const FVector DefaultLocationOfHandsMeshComponent = FVector(-25, 15, -150.f);
	const FRotator DefaultRotatorOfHandsMeshComponent = FRotator(-7.f, -15.f, 0.f);

	USkeletalMeshComponent* BodyMeshComponent;
	const FVector DefaultLocationOfBodyMeshComponent = FVector(0.f, 0.f, -94.f);
	const FRotator DefaultRotatorOfBodyMeshComponent = FRotator(0.f, -90.f, 0.f);

	// PrimaryWeapon will be loaded at BeginPlay();
	UPROPERTY(Replicated, EditDefaultsOnly)
	class AWeaponBase* PrimaryWeapon;

	class AFPSHUD* HUD;

	UCharacterMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	float MaxHealth;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = Gameplay)
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	float MaxArmor;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = Gameplay)
	float Armor;

	bool bIsDead;

public:
	// Sets default values for this character's properties
	AFPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void TickCrosshair();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Movement function
	void MoveForward(float Value);
	void MoveRight(float Value);
	// Jump() is already made by ACharacter. Don't add.

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/**************************
		 Input event (RPC)
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

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCStartAction();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCStopAction();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCStartSubaction();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCStopSubaction();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCStartReload();

	/**************************
		  Characer status
	***************************/
	UFUNCTION(BlueprintCallable, Category = "Character")
	void Die();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void Respawn();

	/**************************
		   About weapon
	***************************/
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(FString WeaponReferance);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void UnEquipWeapon();

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


private:
	// For hit character
	bool LineTrace(FHitResult& HitResult);
};