// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "../Actors/FPSCharacter.h"
#include "PickupableActor.h"
#include "HandsModelForBody.h"
#include "Net/UnrealNetwork.h"

/*
 * class FPS_API AWeaponBase
 */

// Sets default values
AWeaponBase::AWeaponBase() : AHands()
{
	PrimaryActorTick.bCanEverTick = true;

	// properties
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
	MovementStability = 1.f;
	Damage = 40;
	RecoilRecoveryTime = 0.3f;
	CurrentRecoilRecoveryTime = 0.f;

	// Animation instance
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

	// Timeline
	CameraRecoilControlCurve = nullptr;
	BulletRecoilCurve = nullptr;
	CameraRecoilStabilityCurve = nullptr;
	MaximumCameraRecoilControl = FVector(0.f, 0.f, 0.f);
	CurrentCameraRecoilControl = FVector(0.f, 0.f, 0.f);
	MaximumBulletRecoil = FVector(0.f, 0.f, 0.f);
	CurrentBulletRecoil = FVector(0.f, 0.f, 0.f);
	MaximumHoldTime = 0.f;
	CurrentHoldTime = 0.f;
}

void AWeaponBase::Initialize(AFpsCharacter* FpsCharacter)
{
	Super::Initialize(FpsCharacter);
	if (!IsValid(FpsCharacter)) return;
	SetBodyAnimInstance(FpsCharacter->GetBodyMeshComponent()->GetAnimInstance());
}

void AWeaponBase::InitializeRecoilTimeline()
{
	if (CameraRecoilControlCurve == nullptr || BulletRecoilCurve == nullptr || CameraRecoilStabilityCurve == nullptr)
	{
		return;
	}

	FOnTimelineVector CameraRecoilControlCallback;
	FOnTimelineVector BulletRecoilCallback;

	FOnTimelineEventStatic TimelineFinishCallback;

	CameraRecoilControlCallback.BindUFunction(this, FName("OnCameraRecoilControlProgress"));
	BulletRecoilCallback.BindUFunction(this, FName("OnBulletRecoilProgress"));
	TimelineFinishCallback.BindUFunction(this, FName("OnRecoilControlTimelineFinish"));

	RecoilControlTimeline.AddInterpVector(CameraRecoilControlCurve, CameraRecoilControlCallback);
	RecoilControlTimeline.AddInterpVector(BulletRecoilCurve, BulletRecoilCallback);
	RecoilControlTimeline.SetTimelineFinishedFunc(TimelineFinishCallback);

	FOnTimelineVector CameraRecoilStabilityCallback;
	CameraRecoilStabilityCallback.BindUFunction(this, FName("OnCameraRecoilStabilityProgress"));
	RecoilStabilityTimeline.AddInterpVector(CameraRecoilStabilityCurve, CameraRecoilStabilityCallback);
}


// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeRecoilTimeline();
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOnAutomaticRecoil)
	{
		AutomaticRecoilControlTick(DeltaTime);
	}
	else
	{
		RecoilControlTick(DeltaTime);
	}
	RecoilStabilityTick(DeltaTime);
}

void AWeaponBase::RecoilControlTick(float DeltaTime)
{
	RecoilControlTimeline.TickTimeline(DeltaTime);
	if (RecoilControlTimeline.IsPlaying())
	{
		CurrentRecoilRecoveryTime += DeltaTime;
		if (CurrentRecoilRecoveryTime > RecoilRecoveryTime) CurrentRecoilRecoveryTime = RecoilRecoveryTime;
	}
	else if (0.f <= CurrentRecoilRecoveryTime)
	{
		CurrentRecoilRecoveryTime -= DeltaTime;
		float ReducedTimelinePlayback = RecoilControlTimeline.GetPlaybackPosition() * (CurrentRecoilRecoveryTime / RecoilRecoveryTime);
		RecoilControlTimeline.SetPlaybackPosition(ReducedTimelinePlayback, false);
	}
}

void AWeaponBase::RecoilStabilityTick(float DeltaTime)
{
	RecoilStabilityTimeline.TickTimeline(DeltaTime / ActionDelay);
}

#define SPRAY_SPEED 10.f
void AWeaponBase::AutomaticRecoilControlTick(float DeltaTime)
{
	// If is the side to aim and the current aim is over that.
	if ((0.f < MaximumCameraRecoilControl.Z && MaximumCameraRecoilControl.Z <= CurrentCameraRecoilControl.Z)
		|| (MaximumCameraRecoilControl.Z < 0.f && CurrentCameraRecoilControl.Z <= MaximumCameraRecoilControl.Z))
	{
		UE_LOG(LogTemp, Log, TEXT("AWeaponBase::AutomaticRecoilControlTick over"));
		if (MaximumHoldTime == 0.f)
		{
			MaximumHoldTime = FMath::RandRange(0.3f, 0.7f);
			UE_LOG(LogTemp, Log, TEXT("MaximumHoldTime : %f "), MaximumHoldTime);
		}
		
		CurrentHoldTime += DeltaTime;

		if (MaximumHoldTime < CurrentHoldTime)
		{
			MaximumHoldTime = 0.f;
			CurrentHoldTime = 0.f;
			MaximumCameraRecoilControl.Z *= -1.f;
			MaximumBulletRecoil.Z *= -1.f;
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AWeaponBase::AutomaticRecoilControlTick turn"));
		float SprayDelta = SPRAY_SPEED * DeltaTime;
		float SprayValue = MaximumCameraRecoilControl.Z - CurrentCameraRecoilControl.Z;
		if (SprayValue * SprayValue < 1.f) 
			SprayValue = SprayValue < 0 ? -1.f : 1.f;
		CurrentCameraRecoilControl.Z += SprayValue * SprayDelta;


		SprayValue = MaximumBulletRecoil.Z - CurrentBulletRecoil.Z;
		if (SprayValue * SprayValue < 1.f)
			SprayValue = SprayValue < 0 ? -1.f : 1.f;
		CurrentBulletRecoil.Z += SprayValue * SprayDelta;
	}

	for (IWeaponEvent* Observer : EventObservers)
	{
		Observer->OnCameraRecoilControlProgress(CurrentCameraRecoilControl);
		Observer->OnBulletRecoilProgress(CurrentBulletRecoil);
	}
}
#undef SPRAY_SPEED

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponBase, CurrentAmmo);
	DOREPLIFETIME(AWeaponBase, SubAmmo);
}

void AWeaponBase::AddObserver(IWeaponEvent* Observer)
{
	EventObservers.Add(Observer);
}

void AWeaponBase::RemoveObserver(IWeaponEvent* Observer)
{
	EventObservers.Remove(Observer);
}

void AWeaponBase::OnUnequipHands(AHands* Hands)
{
	Super::OnUnequipHands(Hands);
	SetBodyAnimInstance(NULL);
	StopAction();
	StopSubaction();
}

void AWeaponBase::OnActionPressed()
{
	StartAction();
}

void AWeaponBase::OnActionReleased()
{
	StopAction();
}

void AWeaponBase::OnSubactionPressed()
{
	StartSubaction();
}

void AWeaponBase::OnSubactionReleased()
{
	StopSubaction();
}

void AWeaponBase::OnReloadPressed()
{
	StartReload();
}

void AWeaponBase::OnCameraRecoilControlProgress(FVector CameraRecoil)
{
	for (IWeaponEvent* Observer : EventObservers)
	{
		Observer->OnCameraRecoilControlProgress(CameraRecoil);
	}

	MaximumCameraRecoilControl.X = CameraRecoil.X > MaximumCameraRecoilControl.X ? CameraRecoil.X : MaximumCameraRecoilControl.X;
	MaximumCameraRecoilControl.Y = CameraRecoil.Y > MaximumCameraRecoilControl.Y ? CameraRecoil.Y : MaximumCameraRecoilControl.Y;
	MaximumCameraRecoilControl.Z = CameraRecoil.Z > MaximumCameraRecoilControl.Z ? CameraRecoil.Z : MaximumCameraRecoilControl.Z;
	CurrentCameraRecoilControl = CameraRecoil;
}

void AWeaponBase::OnBulletRecoilProgress(FVector BulletRecoil)
{
	for (IWeaponEvent* Observer : EventObservers)
	{
		Observer->OnBulletRecoilProgress(BulletRecoil);
	}

	MaximumBulletRecoil.X = MaximumBulletRecoil.X > MaximumBulletRecoil.X ? MaximumBulletRecoil.X : MaximumBulletRecoil.X;
	MaximumBulletRecoil.Y = MaximumBulletRecoil.Y > MaximumBulletRecoil.Y ? MaximumBulletRecoil.Y : MaximumBulletRecoil.Y;
	MaximumBulletRecoil.Z = MaximumBulletRecoil.Z > MaximumBulletRecoil.Z ? MaximumBulletRecoil.Z : MaximumBulletRecoil.Z;
	CurrentBulletRecoil = BulletRecoil;
}

void AWeaponBase::OnRecoilControlTimelineFinish()
{
	UE_LOG(LogTemp, Log, TEXT("AWeaponBase::OnRecoilControlTimelineFinish"));
	//Start automatic recoil left and right.
	IsOnAutomaticRecoil = true;
}

void AWeaponBase::OnCameraRecoilStabilityProgress(FVector CameraRecoil)
{
	for (IWeaponEvent* Observer : EventObservers)
	{
		Observer->OnCameraRecoilStabilityProgress(CameraRecoil);
	}
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

	RecoilControlTimeline.Play();
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

	RecoilControlTimeline.Stop();
	for (IWeaponEvent* Observer : EventObservers)
	{
		Observer->OnRecoilStop(RecoilRecoveryTime);
	}

	IsOnAutomaticRecoil = false;
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

void AWeaponBase::StartReload()
{
	UE_LOG(LogTemp, Log, TEXT("StartReload()"));

	// return if is on delay or ammo is full
	if (FunctionAfterDelay == &AWeaponBase::OnReload 
		|| CurrentAmmo == MagazineSize) return;

	FunctionAfterDelay = &AWeaponBase::OnReload;
	GetWorldTimerManager().SetTimer(TimerHandle, this, FunctionAfterDelay, ReloadDelay, false);

	// Play reloading animation
	if (BodyReloadAnimation != NULL && BodyAnimInstance != NULL) BodyAnimInstance->Montage_Play(BodyReloadAnimation);
	else UE_LOG(LogTemp, Log, TEXT("BodyReloadAnimation or BodyAnimInstance is NULL"));

	// Play reloading sound
	if(ReloadSound) 
		UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
}

void AWeaponBase::OnAction()
{
	UE_LOG(LogTemp, Log, TEXT("AWeaponBase::OnAction()"));
	if (CurrentAmmo <= 0) return;
	CurrentAmmo -= !IsAmmoInfinite;

	MulticastRPCOnActionFx();

	RecoilStabilityTimeline.SetPlaybackPosition(0.f, false);
	if (!RecoilStabilityTimeline.IsPlaying()) RecoilStabilityTimeline.Play();

	for (IWeaponEvent* Observer : EventObservers)
	{
		Observer->OnActionEvent(this);
	}
}

void AWeaponBase::MulticastRPCOnActionFx_Implementation()
{
	// Play animations
	UAnimInstance* WeaponAnimInstance = HandsMesh->GetAnimInstance();
	if (IsValid(WeaponAnimInstance))
	{
		UE_LOG(LogTemp, Log, TEXT("WeaponAnimInstance play HandsActionAnimation"));
		WeaponAnimInstance->Montage_Play(HandsActionAnimation);
	}
	else UE_LOG(LogTemp, Log, TEXT("WeaponAnimInstance is Invalid"));

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
	if (BodySubactionAnimation != NULL && BodyAnimInstance != NULL) BodyAnimInstance->Montage_Play(BodySubactionAnimation);
	else UE_LOG(LogTemp, Log, TEXT("BodySubactionAnimation or BodyAnimInstance is NULL"));

	// Play sound
	if (SubactionSound != NULL)
		UGameplayStatics::PlaySoundAtLocation(this, SubactionSound, GetActorLocation());

	for (IWeaponEvent* Observer : EventObservers)
	{
		Observer->OnSubactionEvent(this);
	}
}

void AWeaponBase::OnReload()
{
	UE_LOG(LogTemp, Log, TEXT("OnReload()"));

	int RequiredAmmo = MagazineSize - CurrentAmmo;
	int ChargedAmmo = (RequiredAmmo < SubAmmo) ? RequiredAmmo : SubAmmo;
	SubAmmo -= ChargedAmmo;
	CurrentAmmo += ChargedAmmo;

	for (IWeaponEvent* Observer : EventObservers)
	{
		Observer->OnReloadEvent(this);
	}
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

void AWeaponBase::SetBodyAnimInstance(UAnimInstance* Instance)
{
	BodyAnimInstance = Instance;
}