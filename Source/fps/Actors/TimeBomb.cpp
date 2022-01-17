// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeBomb.h"
#include "Net/UnrealNetwork.h"
#include "FpsCharacter.h"

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
	if (GetNetMode() == NM_Client) return;

	CurrentTime = MaxTime;
	SetActorTickEnabled(true);
}

void ATimeBomb::Tick(float DeltaTime)
{
	CurrentTime -= DeltaTime;
	if (CurrentTime <= 0.f)
	{
		CurrentTime = 0;
		SetActorTickEnabled(false);
		// TODO : Event for explosion
	}
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
	if (!IsValid(FpsCharcter))
	{
		return;
	}
	FpsCharacter->SetCharacterStatus(EFpsCharacterStatus::Freeze);
	IsDismantiling = true;
}

void ATimeBomb::OnInteractionStop(ACharacter* character)
{
	UE_LOG(LogTemp, Log, TEXT("ATimeBomb::OnInteractionStop"));
	AFpsCharacter* FpsCharacter = Cast<AFpsCharacter>(character);
	if (!IsValid(FpsCharcter))
	{
		return;
	}
	FpsCharacter->SetCharacterStatus(EFpsCharacterStatus::Alive);
	IsDismantiling = false;
}

float ATimeBomb::GetCurrentTime()
{
	return CurrentTime;
}