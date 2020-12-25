// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WeaponInputInterface.h"
#include "FPSCharacter.generated.h"

UCLASS()
class FPS_API AFPSCharacter : public ACharacter, public IWeaponInputInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* HandsMeshComponent;

	// PrimaryWeapon will be loaded at BeginPlay();
	UPROPERTY(EditDefaultsOnly)
	class AWeaponBase* PrimaryWeapon;

	class AFPSHUD* HUD;

	UCharacterMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	float MaxHealth;
	
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	float MaxArmor;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
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

	/**************************
		IWeaponInputInterface
	***************************/
	virtual void StartAction() override;
	virtual void StopAction() override;
	virtual void StartSubaction() override;
	virtual void StopSubaction() override;
	virtual void Reload() override;

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
};