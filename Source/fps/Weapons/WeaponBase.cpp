// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "../Actors/Characters/FPSCharacter.h"
#include "PickUpWeapon.h"
#include "WeaponModelForBody.h"
#include "Net/UnrealNetwork.h"

/*
 * class FPS_API AWeaponBase
 */

// Sets default values
AWeaponBase::AWeaponBase()
{
	bReplicates = true;

 	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;	
	// Components
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	WeaponMesh->SetOnlyOwnerSee(true);
	RootComponent = WeaponMesh;

	// properties
	AttachingGripPointName = "GripPoint";
	WeaponType = EWeaponType::Rifle;
	ActionDelay = 0.125f;
	ActionLoopEnable = true;
	SubactionDelay = 1.f;
	SubactionLoopEnable = true;
	ReloadDelay = 2.5f;
	MagazineSize = 30;
	CurrentAmmo = 30;
	SubAmmo = 90;
	IsAmmoInfinite = false;
	Accuracy = 1.f;
	MovementStability = 40;
	Damage = 40;

	// Animation instance
	HandsAnimInstance = NULL;
	BodyAnimInstance = NULL;

	// Animations
	HandsActionAnimation = NULL;
	HandsSubactionAnimation = NULL;
	HandsReloadAnimation = NULL;
	BodyActionAnimation = NULL;
	BodySubactionAnimation = NULL;
	BodyReloadAnimation = NULL;

	// Sounds
	ActionSound = NULL;
	SubactionSound = NULL;
	ReloadSound = NULL;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponBase, CurrentAmmo);
	DOREPLIFETIME(AWeaponBase, SubAmmo);
}

void AWeaponBase::Initialize(AFpsCharacter* FPSCharacter)
{
	SetOwner(FPSCharacter);
	SetHandsAnimInstance(FPSCharacter->GetHandsMeshComponent()->GetAnimInstance());
	SetBodyAnimInstance(FPSCharacter->GetBodyMeshComponent()->GetAnimInstance());
}

void AWeaponBase::OnUnEquipped()
{
	SetOwner(NULL);
	SetHandsAnimInstance(NULL);
	SetBodyAnimInstance(NULL);
}

void AWeaponBase::StartAction()
{
	//Retry StartAction() after delay.
	float Delay = GetDelay();
	if (0.f < Delay)
	{
		FunctionAfterDelayForExtraInput = &AWeaponBase::StartAction;
		GetWorldTimerManager().SetTimer(TimerHandleForExtraInput, this, FunctionAfterDelayForExtraInput, Delay, false);
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("StartAction()"));

	FunctionAfterDelay = &AWeaponBase::OnAction;
	GetWorldTimerManager().SetTimer(TimerHandle, this, FunctionAfterDelay, ActionDelay, ActionLoopEnable, 0.f);
}

void AWeaponBase::StartSubaction()
{
	if (SubAmmo <= 0) return;

	//Retry StartAction() after delay.
	float Delay = GetDelay();
	if (0.f < Delay)
	{
		FunctionAfterDelayForExtraInput = &AWeaponBase::StartSubaction;
		GetWorldTimerManager().SetTimer(TimerHandleForExtraInput, this, FunctionAfterDelayForExtraInput, Delay, false);
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("StartSubaction()"));

	FunctionAfterDelay = &AWeaponBase::OnSubaction;
	GetWorldTimerManager().SetTimer(TimerHandle, this, FunctionAfterDelay, SubactionDelay, SubactionLoopEnable, 0.f);
}

void AWeaponBase::StartReload()
{
	UE_LOG(LogTemp, Log, TEXT("StartReload()"));

	// return if is on delay or ammo is full
	if (FunctionAfterDelay == &AWeaponBase::OnReload 
		|| CurrentAmmo == MagazineSize) return;

	FunctionAfterDelay = &AWeaponBase::OnReload;
	GetWorldTimerManager().SetTimer(TimerHandle, this, FunctionAfterDelay, ReloadDelay, false);

	// Play reloading animation
	if (HandsReloadAnimation != NULL && HandsAnimInstance != NULL) HandsAnimInstance->Montage_Play(HandsReloadAnimation);
	else UE_LOG(LogTemp, Log, TEXT("HandsReloadAnimation or HandsAnimInstance is NULL"));
	if (BodyReloadAnimation != NULL && BodyAnimInstance != NULL) BodyAnimInstance->Montage_Play(BodyReloadAnimation);
	else UE_LOG(LogTemp, Log, TEXT("BodyReloadAnimation or BodyAnimInstance is NULL"));

	// Play reloading sound
	if(ReloadSound) 
		UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
}

void AWeaponBase::StopAction()
{
	if (FunctionAfterDelayForExtraInput == &AWeaponBase::StartAction)
	{
		FunctionAfterDelayForExtraInput = NULL;
		GetWorldTimerManager().ClearTimer(TimerHandleForExtraInput);
	}

	if (FunctionAfterDelay != &AWeaponBase::OnAction) return;

	UE_LOG(LogTemp, Log, TEXT("StopAction()"));

	FunctionAfterDelay = NULL;
	float Remaining = GetWorldTimerManager().GetTimerRemaining(TimerHandle);
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([]()
		{
			//Do nothing. It is just waiting for delay.
		}
	), Remaining, false);
}

void AWeaponBase::StopSubaction()
{
	if (FunctionAfterDelayForExtraInput == &AWeaponBase::StartSubaction)
	{
		FunctionAfterDelayForExtraInput = NULL;
		GetWorldTimerManager().ClearTimer(TimerHandleForExtraInput);
	}

	if (FunctionAfterDelay != &AWeaponBase::OnSubaction) return;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("StopSubaction()"));

	FunctionAfterDelay = NULL;
	float Remaining = GetWorldTimerManager().GetTimerRemaining(TimerHandle);
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([]()
		{
			//Do nothing. just wait for delay.
		}
	), Remaining, false);
}

void AWeaponBase::OnAction()
{
	UE_LOG(LogTemp, Log, TEXT("AWeaponBase::OnAction()"));
	if (CurrentAmmo <= 0) return;
	CurrentAmmo -= !IsAmmoInfinite;

	MulticastRPCOnActionFx();
}

void AWeaponBase::MulticastRPCOnActionFx_Implementation()
{
	// Play animations
	if (HandsActionAnimation != NULL && HandsAnimInstance != NULL)
		HandsAnimInstance->Montage_Play(HandsActionAnimation);
	else
		UE_LOG(LogTemp, Log, TEXT("HandsActionAnimation or HandsAnimInstance is NULL"));
	if (BodyActionAnimation != NULL && BodyAnimInstance != NULL)
		BodyAnimInstance->Montage_Play(BodyActionAnimation);
	else UE_LOG(LogTemp, Log, TEXT("BodyActionAnimation or BodyAnimInstance is NULL"));

	// Play sound
	if (ActionSound != NULL)
		UGameplayStatics::PlaySoundAtLocation(this, ActionSound, GetActorLocation());
	else
		UE_LOG(LogTemp, Log, TEXT("ActionSound NULL"));
}

void AWeaponBase::OnSubaction()
{
	UE_LOG(LogTemp, Log, TEXT("OnSubaction()"));

	// Play animation
	if (HandsSubactionAnimation != NULL && HandsAnimInstance != NULL) HandsAnimInstance->Montage_Play(HandsSubactionAnimation);
	else UE_LOG(LogTemp, Log, TEXT("HandsSubactionAnimation or HandsAnimInstance is NULL"));
	if (BodySubactionAnimation != NULL && BodyAnimInstance != NULL) BodyAnimInstance->Montage_Play(BodySubactionAnimation);
	else UE_LOG(LogTemp, Log, TEXT("BodySubactionAnimation or BodyAnimInstance is NULL"));

	// Play sound
	if (SubactionSound != NULL)
		UGameplayStatics::PlaySoundAtLocation(this, SubactionSound, GetActorLocation());
}

void AWeaponBase::OnReload()
{
	UE_LOG(LogTemp, Log, TEXT("OnReload()"));

	int RequiredAmmo = MagazineSize - CurrentAmmo;
	int ChargedAmmo = (RequiredAmmo < SubAmmo) ? RequiredAmmo : SubAmmo;
	SubAmmo -= ChargedAmmo;
	CurrentAmmo += ChargedAmmo;
}

float AWeaponBase::GetDelay()
{
	return GetWorldTimerManager().GetTimerRemaining(TimerHandle);
}

float AWeaponBase::GetMovementStability()
{
	return MovementStability;
}

int AWeaponBase::GetCurrentAmmo()
{
	return CurrentAmmo;
}

int AWeaponBase::GetSubAmmo()
{
	return SubAmmo;
}

FName AWeaponBase::GetAttachingGripPointName()
{
	return AttachingGripPointName;
}

EWeaponType AWeaponBase::GetWeaponType()
{
	return WeaponType;
}

void AWeaponBase::SetHandsAnimInstance(UAnimInstance* Instance)
{
	HandsAnimInstance = Instance;
}

void AWeaponBase::SetBodyAnimInstance(UAnimInstance* Instance)
{
	BodyAnimInstance = Instance;
}

APickUpWeapon* AWeaponBase::SpawnPickUpWeaponActor()
{
	FRotator Rotation = GetActorRotation();
	return GetWorld()->SpawnActor<APickUpWeapon>(PickUpWeaponSubclass, GetActorLocation(), FRotator(90, Rotation.Yaw, 0));
}

AWeaponModelForBody* AWeaponBase::SpawnWeaponModelForBodyActor()
{
	return GetWorld()->SpawnActor<AWeaponModelForBody>(WeaponModelForBodySubclass, FVector(0, 0, 0), FRotator::ZeroRotator);
}

AWeaponBase* AWeaponBase::SpawnWeapon(UWorld* World, UClass* GeneratedBP)
{
	FActorSpawnParameters SpawnParameters;
	return World->SpawnActor<AWeaponBase>(GeneratedBP, FVector(0, 0, 0), FRotator::ZeroRotator, SpawnParameters);
}