// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupableActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Math/UnrealMathUtility.h"
#include "../Actors/FpsCharacter.h"
#include "Hands.h"

// Sets default values
APickupableActor::APickupableActor()
{
	// For replication
	bReplicates = true;
	SetReplicateMovement(true);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UStaticMeshComponent"));
	RootComponent = Mesh;
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionObjectType(ECC_WorldDynamic);
	Mesh->SetCollisionResponseToChannels(ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->OnComponentHit.AddDynamic(this, &APickupableActor::OnWeaponMeshComponentHit);
}

// Called when the game starts or when spawned
void APickupableActor::BeginPlay()
{
	Super::BeginPlay();
}


void APickupableActor::OnTargetedBy(ACharacter* character)
{
	UE_LOG(LogTemp, Log, TEXT("APickupableActor::OnTargetedBy"));
	AFpsCharacter* Character = Cast<AFpsCharacter>(character);
	if (!IsValid(Character)) 
		return;
}

void APickupableActor::OnUntargeted(ACharacter* character)
{
	UE_LOG(LogTemp, Log, TEXT("APickupableActor::OnUntargeted"));
	AFpsCharacter* Character = Cast<AFpsCharacter>(character);
	if (!IsValid(Character)) return;
}

void APickupableActor::OnInteractWith(ACharacter* character)
{
	UE_LOG(LogTemp, Log, TEXT("APickupableActor::OnInteractWith"));
	AFpsCharacter* Character = Cast<AFpsCharacter>(character);
	if (!IsValid(Character)) return;
	Character->ServerRpcPickUp(this);
}

void APickupableActor::OnInteractionStop(ACharacter* character)
{
	UE_LOG(LogTemp, Log, TEXT("APickupableActor::OnInteractionStop"));
}

void APickupableActor::OnWeaponMeshComponentHit(UPrimitiveComponent* HitComponent,
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

UStaticMeshComponent* APickupableActor::GetMesh()
{
	return Mesh;
}

AHands* APickupableActor::GetHandsInstance()
{
	return HandsInstance;
}

TSubclassOf<AHands> APickupableActor::GetHandsSubclass()
{
	return HandsSubclass;
}

void APickupableActor::SetHandsInstance(AHands* Instance)
{
	HandsInstance = Instance;
}