// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponModelForBody.h"

// Sets default values
AWeaponModelForBody::AWeaponModelForBody()
{
	bReplicates = true;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Components
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	WeaponMesh->SetOwnerNoSee(true);
	RootComponent = WeaponMesh;
	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeaponModelForBody::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponModelForBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

