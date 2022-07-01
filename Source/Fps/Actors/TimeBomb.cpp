// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeBomb.h"
#include "Net/UnrealNetwork.h"
#include "FpsCharacter.h"
#include "../GameMode/PlantBombMode.h"

#include "Kismet/GameplayStatics.h"

ATimeBomb::ATimeBomb()
{
	// For replication
	bReplicates = true;
	SetReplicateMovement(true);

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Components
	BombMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UStaticMeshComponent"));
	BombMeshComponent->SetSimulatePhysics(true);
	BombMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BombMeshComponent->SetCollisionObjectType(ECC_WorldDynamic);
	BombMeshComponent->SetCollisionResponseToChannels(ECR_Block);
	BombMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	BombMeshComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	BombMeshComponent->SetGenerateOverlapEvents(false);
	RootComponent = BombMeshComponent;
}

void ATimeBomb::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("ATimeBomb::BeginPlay"));

	ServerRpcActivate();
}

void ATimeBomb::Tick(float DeltaTime)
{

}

void ATimeBomb::OnTargetedBy(ACharacter* character)
{
	UE_LOG(LogTemp, Log, TEXT("ATimeBomb::OnTargetedBy"));
}

void ATimeBomb::OnUntargeted(ACharacter* character)
{
	UE_LOG(LogTemp, Log, TEXT("ATimeBomb::OnUntargeted"));
}

void ATimeBomb::OnInteractWith(ACharacter* character)
{
	UE_LOG(LogTemp, Log, TEXT("ATimeBomb::OnInteractWith"));
	AFpsCharacter* FpsCharacter = Cast<AFpsCharacter>(character);
	if (!IsValid(FpsCharacter))
	{
		return;
	}
	FpsCharacter->SetCharacterStatus(EFpsCharacterStatus::Freeze);
	IsOnDefusing = true;
}

void ATimeBomb::OnInteractionStop(ACharacter* character)
{
	UE_LOG(LogTemp, Log, TEXT("ATimeBomb::OnInteractionStop"));
	AFpsCharacter* FpsCharacter = Cast<AFpsCharacter>(character);
	if (!IsValid(FpsCharacter))
	{
		return;
	}
	FpsCharacter->SetCharacterStatus(EFpsCharacterStatus::Alive);
	IsOnDefusing = false;
}

void ATimeBomb::Activate()
{
	APlantBombMode* PlantBombMode = Cast<APlantBombMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(PlantBombMode))
	{
		UE_LOG(LogTemp, Log, TEXT("ATimeBomb::Activate PlantBombMode is invalid"));
		return;
	}

	PlantBombMode->OnBombPlant(MaxTime);

	GetWorld()->GetTimerManager().SetTimer(
		ExplosionFxTimer,
		this,
		&ATimeBomb::Explode,
		MaxTime,
		false,
		MaxTime);
}

void ATimeBomb::Explode()
{
	UWorld* World = GetWorld();
	if(ExplosionFx != nullptr) UGameplayStatics::SpawnEmitterAtLocation(World, ExplosionFx, GetTransform());
	if(ExplosionSoundFx != nullptr) UGameplayStatics::PlaySoundAtLocation(World, ExplosionSoundFx, GetActorLocation());
}

void ATimeBomb::ServerRpcActivate_Implementation()
{
	Activate();
}