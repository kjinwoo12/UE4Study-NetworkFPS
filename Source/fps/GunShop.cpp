// Fill out your copyright notice in the Description page of Project Settings.

#include "GunShop.h"
#include "WeaponBase.h"
#include "FpsCharacter.h"
#include "WeaponBase.h"

// Sets default values
AGunShop::AGunShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

}

// Called when the game starts or when spawned
void AGunShop::BeginPlay()
{
	Super::BeginPlay();	
}

bool AGunShop::ServerRpcBuyItem_Validate(TSubclassOf<AWeaponBase> WeaponBlueprint)
{
	return true;
}

void AGunShop::ServerRpcBuyItem_Implementation(TSubclassOf<AWeaponBase> WeaponSubclass)
{
	APlayerController* PlayerController = GetOwner<APlayerController>();
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Log, TEXT("PlayerController is invalid"));
		return;
	}
	AFpsCharacter* FpsCharacter = PlayerController->GetPawn<AFpsCharacter>();
	if (!IsValid(FpsCharacter))
	{
		UE_LOG(LogTemp, Log, TEXT("FpsCharacter is invalid"));
		return;
	}
	AWeaponBase* Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponSubclass);
	FpsCharacter->DropWeapon();
	FpsCharacter->EquipWeapon(Weapon);
}