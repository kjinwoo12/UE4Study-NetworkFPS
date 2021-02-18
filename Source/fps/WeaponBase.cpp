// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "FPSCharacter.h"
#include "PickUpWeapon.h"
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
	RootComponent = WeaponMesh;
	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(RootComponent);

	// Variable
	Reach = 6000.f;
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
	ParentAnimInstance = NULL;

	// Animations
	ActionAnimation = NULL;
	SubactionAnimation = NULL;
	ReloadAnimation = NULL;

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

APickUpWeapon* AWeaponBase::SpawnPickUpWeaponActor()
{
	return (PickUpWeaponBlueprint==NULL)?NULL: GetWorld()->SpawnActor<APickUpWeapon>(PickUpWeaponBlueprint->GeneratedClass, GetActorLocation(), FRotator(0, 0, 0));
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
	UE_LOG(LogTemp, Log, TEXT("Reload()"));

	// return if is on delay
	if (FunctionAfterDelay == &AWeaponBase::OnReload 
		|| CurrentAmmo == MagazineSize
		|| SubAmmo <= 0) return;

	FunctionAfterDelay = &AWeaponBase::OnReload;
	GetWorldTimerManager().SetTimer(TimerHandle, this, FunctionAfterDelay, ReloadDelay, false);

	// Play reloading animation
	if(ReloadAnimation!=NULL) PlayAnimMontage(ReloadAnimation);

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
			//Do nothing. just wait for delay.
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
	if (GetNetMode() == ENetMode::NM_ListenServer)
	{
		UE_LOG(LogTemp, Log, TEXT("OnAction() : Server"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("OnAction() : Client"));
	}

	if (CurrentAmmo <= 0) return;
	CurrentAmmo -= !IsAmmoInfinite;

	MulticastRPCOnActionFx();

	FHitResult HitResult;
	if (!LineTrace(HitResult)) return;
	FPointDamageEvent DamangeEvent;
	DamangeEvent.HitInfo = HitResult;

	HitResult.GetActor()->TakeDamage(Damage, DamangeEvent, PlayerController, this);
}

void AWeaponBase::MulticastRPCOnActionFx_Implementation()
{
	if (GetNetMode() == ENetMode::NM_ListenServer)
	{
		UE_LOG(LogTemp, Log, TEXT("MulticastRPCOnActionFx_Implementation() : Server"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("MulticastRPCOnActionFx_Implementation() : Client"));
	}
	// Play animation
	if (ActionAnimation != NULL) PlayAnimMontage(ActionAnimation);

	// Play sound
	if (ActionSound != NULL)
		UGameplayStatics::PlaySoundAtLocation(this, ActionSound, GetActorLocation());
}

void AWeaponBase::OnSubaction()
{
	UE_LOG(LogTemp, Log, TEXT("OnSubaction()"));

	// Play animation
	if (SubactionAnimation != NULL) PlayAnimMontage(SubactionAnimation);

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

void AWeaponBase::SetParentAnimInstance(UAnimInstance* Instance)
{
	ParentAnimInstance = Instance;
}

void AWeaponBase::SetPlayerController(APlayerController* Instance)
{
	PlayerController = Instance;
}

void AWeaponBase::PlayAnimMontage(UAnimMontage* AnimMontage)
{
	if (ParentAnimInstance == NULL) return;
	ParentAnimInstance->Montage_Play(AnimMontage);
}

float AWeaponBase::GetDelay()
{
	return GetWorldTimerManager().GetTimerRemaining(TimerHandle);
}

FCollisionQueryParams* AWeaponBase::GetLineTraceCollisionQueryParams()
{
	return &LineTraceCollisionQueryParams;
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

bool AWeaponBase::LineTrace(FHitResult& HitResult)
{
	// Get Player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	PlayerController->GetPlayerViewPoint(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);

	// Get end point
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	// Get Muzzle point
	FVector MuzzleLocation = Muzzle->GetComponentLocation();

	//It is for checking line. Player view point to end point
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		5.f,
		0.f,
		1.f
	);

	UE_LOG(LogTemp, Log, TEXT("num : %d"), LineTraceCollisionQueryParams.GetIgnoredActors().Num());

	bool IsHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		PlayerViewPointLocation,
		LineTraceEnd,
		ECollisionChannel::ECC_Pawn,
		LineTraceCollisionQueryParams
	);

	if (!IsHit)
	{
		DrawDebugLine(
			GetWorld(),
			MuzzleLocation,
			LineTraceEnd,
			FColor(0, 255, 0),
			false,
			5.f,
			0.f,
			1.f
		);
	}
	else
	{
		DrawDebugLine(
			GetWorld(),
			MuzzleLocation,
			HitResult.ImpactPoint,
			FColor(0, 255, 0),
			false,
			5.f,
			0.f,
			1.f
		);
	}

	return IsHit;
}

AWeaponBase* AWeaponBase::SpawnWeapon(UWorld* World, FString WeaponReference)
{
	UClass* GeneratedBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *WeaponReference));
	FActorSpawnParameters SpawnParameters;
	return World->SpawnActor<AWeaponBase>(GeneratedBP, FVector(0, 0, 0), FRotator::ZeroRotator, SpawnParameters);
}