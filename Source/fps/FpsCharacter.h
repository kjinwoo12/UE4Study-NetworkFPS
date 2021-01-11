// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WeaponInterface.h"
#include "FPSCharacter.generated.h"

UCLASS()
class FPS_API AFPSCharacter : public ACharacter, public IWeaponInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* HandsMeshComponent;

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
		IWeaponActionInterface
	***************************/
	UFUNCTION(Server, Reliable)
	virtual void StartAction() override;

	UFUNCTION(Server, Reliable)
	virtual void StopAction() override;

	UFUNCTION(Server, Reliable)
	virtual void StartSubaction() override;

	UFUNCTION(Server, Reliable)
	virtual void StopSubaction() override;

	UFUNCTION(Server, Reliable)
	virtual void StartReload() override;

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