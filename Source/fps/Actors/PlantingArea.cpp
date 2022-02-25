// Fill out your copyright notice in the Description page of Project Settings.


#include "PlantingArea.h"
#include "FpsCharacter.h"

// Sets default values
APlantingArea::APlantingArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EventAreaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EventAreaMesh"));
	EventAreaMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EventAreaMesh->SetCollisionResponseToChannels(ECollisionResponse::ECR_Overlap);
	EventAreaMesh->SetVisibility(false, true);
	EventAreaMesh->SetGenerateOverlapEvents(true);
	EventAreaMesh->OnComponentBeginOverlap.AddDynamic(this, &APlantingArea::OnOverlapBegin);
	EventAreaMesh->OnComponentEndOverlap.AddDynamic(this, &APlantingArea::OnOverlapEnd);
	RootComponent = EventAreaMesh;
}

// Called when the game starts or when spawned
void APlantingArea::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlantingArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlantingArea::OnOverlapBegin(
	UPrimitiveComponent* OverlapComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	UE_LOG(LogTemp, Log, TEXT("APlantingArea::OnOverlapBegin - %s"), *OtherActor->GetName());

	AFpsCharacter* FpsCharacter = Cast<AFpsCharacter>(OtherActor);
	if (!IsValid(FpsCharacter))
	{
		UE_LOG(LogTemp, Log, TEXT("APlantingArea::OnOverlapBegin FpsCharacter is invalid"));
		return;
	}

	FpsCharacter->EnablePlanting(true);
}

void APlantingArea::OnOverlapEnd(
	UPrimitiveComponent* OverlapComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex
) {
	UE_LOG(LogTemp, Log, TEXT("APlantingArea::OnOverlapEnd - %s"), *OtherActor->GetName());
	
	AFpsCharacter* FpsCharacter = Cast<AFpsCharacter>(OtherActor);
	if (!IsValid(FpsCharacter))
	{
		UE_LOG(LogTemp, Log, TEXT("APlantingArea::OnOverlapBegin FpsCharacter is invalid"));
		return;
	}
	FpsCharacter->EnablePlanting(false);
}

