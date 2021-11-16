// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunShop.generated.h"

class AWeaponBase;

UCLASS()
class FPS_API AGunShop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunShop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void ServerRpcBuyItem(TSubclassOf<AWeaponBase> WeaponBlueprint);
};
