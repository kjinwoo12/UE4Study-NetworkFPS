// Fill out your copyright notice in the Description page of Project Settings.


#include "EventArea.h"
#include "FpsCharacter.h"

// Sets default values
AEventArea::AEventArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EventAreaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EventAreaMesh"));
	EventAreaMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EventAreaMesh->SetVisibility(false, true);
	EventAreaMesh->SetGenerateOverlapEvents(true);
	EventAreaMesh->OnComponentBeginOverlap.AddDynamic(this, &AEventArea::OnOverlapBegin);
	EventAreaMesh->OnComponentEndOverlap.AddDynamic(this, &AEventArea::OnOverlapEnd);
	RootComponent = EventAreaMesh;
}

// Called when the game starts or when spawned
void AEventArea::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEventArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AEventArea::OnOverlapBegin(
	UPrimitiveComponent* OverlapComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	if (EnteredActors.Contains(OtherActor)) return;
	
	AFpsCharacter* FpsCharacter = Cast<AFpsCharacter>(OtherActor);
	if (!IsValid(FpsCharacter)) return;

	EnteredActors.Add(OtherActor);

	UE_LOG(LogTemp, Log, TEXT("AEventArea::OnOverlapBegin - %s"), *OtherActor->GetName());
}

void AEventArea::OnOverlapEnd(
	UPrimitiveComponent* OverlapComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex
) {
	if (!EnteredActors.Contains(OtherActor)) return;
	UE_LOG(LogTemp, Log, TEXT("AEventArea::OnOverlapEnd - %s"), *OtherActor->GetName());
	EnteredActors.Remove(OtherActor);
}

