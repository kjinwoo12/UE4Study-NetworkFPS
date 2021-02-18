// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpWeapon.h"
#include "Components/SphereComponent.h"
#include "FpsCharacter.h"
#include "WeaponBase.h"

// Sets default values
APickUpWeapon::APickUpWeapon()
{
	bReplicates = true;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Components
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UStaticMeshComponent"));
	RootComponent = WeaponMesh;
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponMesh->SetCollisionResponseToChannels(ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	WeaponMesh->SetGenerateOverlapEvents(false);

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

// Called every frame
void APickUpWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickUpWeapon::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	AFPSCharacter* FpsCharacter = Cast<AFPSCharacter>(OtherActor);
	if (!FpsCharacter) return;

	FpsCharacter->SetPickableWeapon(this);
}

void APickUpWeapon::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex)
{
	AFPSCharacter* FpsCharacter = Cast<AFPSCharacter>(OtherActor);
	if (!FpsCharacter) return;

	FpsCharacter->SetPickableWeapon(NULL);
}

AWeaponBase* APickUpWeapon::GetWeaponInstance()
{
	return WeaponInstance;
}

void APickUpWeapon::SetWeaponInstance(AWeaponBase* Instance)
{
	WeaponInstance = Instance;
}