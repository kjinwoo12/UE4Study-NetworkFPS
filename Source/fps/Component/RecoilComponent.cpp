// Fill out your copyright notice in the Description page of Project Settings.

#include "RecoilComponent.h"

#include "../Actors/FpsCharacter.h"
#include "../Weapons/Hands.h"
#include "../Weapons/WeaponBase.h"

// Sets default values for this component's properties
URecoilComponent::URecoilComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void URecoilComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void URecoilComponent::Initialize(AFpsCharacter* FpsCharacter)
{
	FpsCharacter->AddObserver(this);
}

// Called every frame
void URecoilComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (0.f <= CurrentRecoveryTime)
	{
		FRotator Rotation = GetRelativeRotation();
		float ReducedRecoveryTime = CurrentRecoveryTime - DeltaTime;
		Rotation.Pitch *= ReducedRecoveryTime / CurrentRecoveryTime;
		Rotation.Yaw *= ReducedRecoveryTime / CurrentRecoveryTime;
		Rotation.Roll *= ReducedRecoveryTime / CurrentRecoveryTime;
		SetRelativeRotation(Rotation);
		CurrentRecoveryTime = ReducedRecoveryTime;
	}
}

void URecoilComponent::OnEquipHands(AHands* Hands)
{
	AWeaponBase* WeaponBase = Cast<AWeaponBase>(Hands);
	if (!IsValid(WeaponBase)) return;

	CurrentRecoveryTime = 0.f;

	WeaponBase->AddObserver(this);
}

void URecoilComponent::OnUnequipHands(AHands* Hands)
{
	AWeaponBase* WeaponBase = Cast<AWeaponBase>(Hands);
	if (!IsValid(WeaponBase)) return;

	WeaponBase->RemoveObserver(this);
}

void URecoilComponent::OnActionPressed()
{
	bIsActionPressed = true;
}

void URecoilComponent::OnActionReleased()
{
	bIsActionPressed = false;
}

void URecoilComponent::OnCameraRecoilProgress(FVector CameraRecoil)
{
	SetRelativeRotation(FRotator(CameraRecoil.Y, CameraRecoil.Z, CameraRecoil.X));
}

void URecoilComponent::OnBulletRecoilProgress(FVector BulletRecoil)
{
}

void URecoilComponent::OnRecoilStop(float RecoveryTime)
{
	UE_LOG(LogTemp, Log, TEXT("OnRecoilStop %f"), RecoveryTime);
	CurrentRecoveryTime = RecoveryTime;
}

FRotator URecoilComponent::CalculateCameraRotation(FRotator Base, FRotator ErrorRange)
{
	UE_LOG(LogTemp, Log, TEXT("Base(%f, %f, %f)"), Base.Pitch, Base.Yaw, Base.Roll);
	UE_LOG(LogTemp, Log, TEXT("ErrorRange(%f, %f, %f)"), ErrorRange.Pitch, ErrorRange.Yaw, ErrorRange.Roll);
	return FRotator(
		FMath::RandRange(Base.Pitch, Base.Pitch+ErrorRange.Pitch),
		FMath::RandRange(Base.Yaw, Base.Yaw+ErrorRange.Yaw),
		FMath::RandRange(Base.Roll, Base.Roll+ErrorRange.Roll)
	);
}