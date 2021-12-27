// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Math/UnrealMathUtility.h"
#include "../Actors/FpsCharacter.h"
#include "WeaponBase.h"

// Sets default values
APickUpWeapon::APickUpWeapon()
{
	// For replication
	bReplicates = true;
	SetReplicateMovement(true);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Components
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UStaticMeshComponent"));
	RootComponent = WeaponMesh;
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponMesh->SetCollisionResponseToChannels(ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	WeaponMesh->SetGenerateOverlapEvents(false);
	WeaponMesh->SetNotifyRigidBodyCollision(true);
	WeaponMesh->OnComponentHit.AddDynamic(this, &APickUpWeapon::OnWeaponMeshComponentHit);

	PickUpRange = CreateDefaultSubobject<USphereComponent>(TEXT("USphereComponent"));
	PickUpRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickUpRange->SetupAttachment(WeaponMesh);
	PickUpRange->SetGenerateOverlapEvents(true);
	PickUpRange->OnComponentBeginOverlap.AddDynamic(this, &APickUpWeapon::OnOverlapBegin);
	PickUpRange->OnComponentEndOverlap.AddDynamic(this, &APickUpWeapon::OnOverlapEnd);
}

// Called when the game starts or when spawned
void APickUpWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void APickUpWeapon::OnWeaponMeshComponentHit(UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse,
	const FHitResult& HitResult)
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		return;
	}
	
	FVector Direction;
	float Speed;
	GetVelocity().ToDirectionAndLength(Direction, Speed);

	const float ClampRangeA = 500.f;
	const float ClampRangeB = 30.f;
	const float OutRangeA = 0;
	const float OutRangeB = 5;

	float ClampedValue = FMath::Clamp(Speed, ClampRangeB, ClampRangeA);
	float InRange = ClampRangeA - ClampRangeB;
	float CorrectInValue = ClampedValue - ClampRangeB;
	float InPercentage = CorrectInValue / InRange;
	float MapRangedClamped = (InPercentage * (OutRangeA - OutRangeB)) + OutRangeB;
	UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, DropSound, HitResult.Location);
	AudioComponent->SetIntParameter("Power", MapRangedClamped);
}

UStaticMeshComponent* APickUpWeapon::GetWeaponMesh()
{
	return WeaponMesh;
}

AWeaponBase* APickUpWeapon::GetWeaponInstance()
{
	return WeaponInstance;
}

TSubclassOf<AWeaponBase> APickUpWeapon::GetWeaponBaseSubclass()
{
	return WeaponBaseSubclass;
}

void APickUpWeapon::SetWeaponInstance(AWeaponBase* Instance)
{
	WeaponInstance = Instance;
}