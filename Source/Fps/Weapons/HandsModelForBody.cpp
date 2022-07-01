// Fill out your copyright notice in the Description page of Project Settings.


#include "HandsModelForBody.h"
#include "../Actors/FpsCharacter.h"

// Sets default values
AHandsModelForBody::AHandsModelForBody()
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

	// Properties
	AttachingGripPointName = "GripPoint";
}

// Called when the game starts or when spawned
void AHandsModelForBody::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHandsModelForBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHandsModelForBody::Initialize(AActor* Parent)
{
	SetOwner(Parent);
	AFpsCharacter* FpsCharacter = Cast<AFpsCharacter>(Parent);
	if (!IsValid(FpsCharacter))
	{
		UE_LOG(LogTemp, Log, TEXT("AHandsModelForBody::Initialize FpsCharacter is invalid"));
		return;
	}
}

FName AHandsModelForBody::GetAttachingGripPointName()
{
	return AttachingGripPointName;
}