// Fill out your copyright notice in the Description page of Project Settings.

#include "FpsCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"

#include "../Weapons/Hands.h"
#include "../Weapons/WeaponBase.h"
#include "../Weapons/PickupableActor.h"
#include "../Weapons/HandsModelForBody.h"
#include "../Ui/FpsCharacterHud.h"
#include "../Ui/FpsCharacterWidget.h"
#include "../PlayerController/FpsPlayerController.h"
#include "../PlayerController/FpsPlayerState.h"
#include "../Component/RecoilComponent.h"

#include "GunShop.h"
#include "InteractiveActor.h"

#include <algorithm>

// Sets default values
AFpsCharacter::AFpsCharacter()
{
	bReplicates = true;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitializeCollisionComponent();
	InitializeMovementComponent();
	InitializeCameraJointComponent();
	InitializeRecoilComponent();
	InitializeCamera();
	InitializeBodyMesh();
	InitializeGameplayVariable();
}

void AFpsCharacter::InitializeCollisionComponent()
{
	UCapsuleComponent* CharacterCapsuleComponent = GetCapsuleComponent();
	CharacterCapsuleComponent->SetCapsuleHalfHeight(94.0f);
	CharacterCapsuleComponent->SetCapsuleRadius(42.0f);
	CharacterCapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	CharacterCapsuleComponent->SetCollisionProfileName("Pawn");
}

void AFpsCharacter::InitializeMovementComponent()
{
	MovementComponent = ACharacter::GetCharacterMovement();
	MovementComponent->bUseFlatBaseForFloorChecks = true;
	MovementComponent->MaxWalkSpeed = 400;
	MovementComponent->MaxWalkSpeedCrouched = 200;
	MovementComponent->NavAgentProps.bCanCrouch = true;
}

void AFpsCharacter::InitializeCameraJointComponent()
{
	CameraYawJointComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CameraYawJointComponent"));
	CameraYawJointComponent->SetupAttachment(GetCapsuleComponent());
	CameraYawJointComponent->SetRelativeLocation(FVector(0.f, 0.f, BaseEyeHeight));

	CameraPitchJointComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CameraPitchJointComponent"));
	CameraPitchJointComponent->SetupAttachment(CameraYawJointComponent);
}

void AFpsCharacter::InitializeRecoilComponent()
{
	RecoilComponent = CreateDefaultSubobject<URecoilComponent>(TEXT("RecoilComponent"));
	RecoilComponent->SetupAttachment(CameraPitchJointComponent);
	RecoilComponent->SetIsReplicated(true);
	RecoilComponent->Initialize(this);
}

void AFpsCharacter::InitializeCamera()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RecoilComponent);
	CameraComponent->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = false;
}

void AFpsCharacter::InitializeBodyMesh()
{
	BodyMeshComponent = GetMesh();
	BodyMeshComponent->SetIsReplicated(true);
	BodyMeshComponent->SetOwnerNoSee(true);
	BodyMeshComponent->SetRelativeLocation(DefaultLocationOfBodyMeshComponent);
	BodyMeshComponent->SetRelativeRotation(DefaultRotatorOfBodyMeshComponent);
	BodyMeshComponent->SetCollisionProfileName("Ragdoll");
}

void AFpsCharacter::InitializeGameplayVariable()
{
	CharacterStatus = EFpsCharacterStatus::Alive;
	MaxHealth = 100;
	Health = 100;
	MaxArmor = 100;
	Armor = 50;
	BodyMeshAimPitch = 0;
	BodyMeshAimYaw = 0;
}

void AFpsCharacter::InitializeGunShop()
{
	if (!IsValid(GunShop))
	{
		GunShop = GetWorld()->SpawnActor<AGunShop>();
	}
	if (!IsValid(GunShop)) 
	{
		UE_LOG(LogTemp, Log, TEXT("GunShop is invalid"));
		return;
	}
	GunShop->SetOwner(this);
}

// Called when the game starts or when spawned
void AFpsCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("FpsCharacterIsSpawned"));

	Inventory.Init(nullptr, 5);
}

void AFpsCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFpsCharacter, Health);
	DOREPLIFETIME(AFpsCharacter, Armor);
	DOREPLIFETIME(AFpsCharacter, InteractiveTarget);
	DOREPLIFETIME(AFpsCharacter, Hands);
	DOREPLIFETIME(AFpsCharacter, HandsModelForBody);
	DOREPLIFETIME(AFpsCharacter, CharacterStatus);
	DOREPLIFETIME(AFpsCharacter, BodyMeshAimPitch);
	DOREPLIFETIME(AFpsCharacter, BodyMeshAimYaw);
	DOREPLIFETIME(AFpsCharacter, GunShop);
	DOREPLIFETIME(AFpsCharacter, CurrentHandsIndex);
	DOREPLIFETIME(AFpsCharacter, Inventory);
	DOREPLIFETIME(AFpsCharacter, Plantable);
}

// Called every frame
void AFpsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateBodyMeshAimOffset(DeltaTime);
	UpdateCameraRotation();
	UpdateCrosshair();
	UpdateInteractiveTarget(DeltaTime);
}

void AFpsCharacter::UpdateCrosshair()
{
	if (GetNetMode() == NM_DedicatedServer) return;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!IsValid(PlayerController) || !IsValid(Hands)) return;

	AFpsCharacterHud* FpsHud = Cast<AFpsCharacterHud>(PlayerController->GetHUD());
	if (!IsValid(FpsHud)) return;

	AWeaponBase* PrimaryWeapon = Cast<AWeaponBase>(Hands);
	if (!IsValid(PrimaryWeapon))
	{
		FpsHud->SetCrosshairCenterOffset(0.f);
		return;
	}

	const float CharacterSpeedOffset = GetVelocity().Size() / PrimaryWeapon->GetMovementStability();
	const int JumpingOffset = (MovementComponent->IsFalling()) ? 30 : 0;
	const float CrosshairCenterOffset = CharacterSpeedOffset + JumpingOffset;
	FpsHud->SetCrosshairCenterOffset(CrosshairCenterOffset);
}

bool AFpsCharacter::ServerRpcSetCameraRotation_Validate(FRotator CameraRotation)
{
	return true;
}

void AFpsCharacter::ServerRpcSetCameraRotation_Implementation(FRotator CameraRotation)
{
	CameraComponent->SetWorldRotation(CameraRotation);
}

void AFpsCharacter::UpdateBodyMeshAimOffset(float DeltaTime)
{
	if (GetNetMode() == NM_Client) return;

	FRotator ControlRotation = GetControlRotation();

	if (!GetVelocity().IsZero())
	{
		SetActorRotation(FRotator(0.f, ControlRotation.Yaw, 0.f));
		BodyMeshAimYaw = 0.f;
		return;
	}

	FRotator AimRotator = FRotator(BodyMeshAimPitch, BodyMeshAimYaw, 0);
	FRotator ActorRotation = GetActorRotation();
	AimRotator = UKismetMathLibrary::RInterpTo(
		AimRotator,
		UKismetMathLibrary::NormalizedDeltaRotator(ControlRotation, ActorRotation),
		DeltaTime,
		0
	);
	BodyMeshAimPitch = UKismetMathLibrary::ClampAngle(AimRotator.Pitch, -90, 90);
	BodyMeshAimYaw = UKismetMathLibrary::ClampAngle(AimRotator.Yaw, -90, 90);

	if (AimRotator.Yaw < -90 || 90 < AimRotator.Yaw)
	{
		AddActorWorldRotation(FRotator(0, AimRotator.Yaw - BodyMeshAimYaw, 0));
	}
}

void AFpsCharacter::UpdateCameraRotation()
{
	FRotator ControlRotation = GetControlRotation();
	CameraYawJointComponent->SetWorldRotation(FRotator(0.f, ControlRotation.Yaw, 0.f));
	CameraPitchJointComponent->SetRelativeRotation(FRotator(ControlRotation.Pitch, 0.f, 0.f));
}

void AFpsCharacter::UpdateInteractiveTarget(float DeltaTime) 
{
	if (GetNetMode() != NM_Client) return;
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());

	if (!IsValid(PlayerController))
	{
		return;
	}

	// Get Player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	PlayerController->GetPlayerViewPoint(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);

	// Get end point
	FHitResult HitResult;
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * InteractionReach;
	bool IsHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		PlayerViewPointLocation,
		LineTraceEnd,
		ECollisionChannel::ECC_Visibility,
		LineTraceCollisionQueryParams
	);
	if (!IsHit)
	{
		if (IsValid(InteractiveTarget))
			InteractiveTarget->OnUntargeted(this);
		InteractiveTarget = nullptr;
		return;
	}

	AInteractiveActor *InteractiveActor = Cast<AInteractiveActor>(HitResult.GetActor());
	if (!IsValid(InteractiveActor))
	{
		if (IsValid(InteractiveTarget))
			InteractiveTarget->OnUntargeted(this);
		InteractiveTarget = nullptr;
		return;
	}

	if (InteractiveTarget == InteractiveActor)
	{
		return;
	}
	InteractiveTarget = InteractiveActor;
	InteractiveTarget->OnTargetedBy(this);
}

// Called to bind functionality to input
void AFpsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AFpsCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFpsCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AFpsCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AFpsCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFpsCharacter::Jump);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AFpsCharacter::CrouchPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AFpsCharacter::CrouchReleased);

	// Hands actions
	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AFpsCharacter::ActionPressed);
	PlayerInputComponent->BindAction("Action", IE_Released, this, &AFpsCharacter::ActionReleased);
	PlayerInputComponent->BindAction("Subaction", IE_Pressed, this, &AFpsCharacter::SubactionPressed);
	PlayerInputComponent->BindAction("Subaction", IE_Released, this, &AFpsCharacter::SubactionReleased);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFpsCharacter::ReloadPressed);
	
	// Interaction
	PlayerInputComponent->BindAction("DropWeapon", IE_Pressed, this, &AFpsCharacter::DropWeaponPressed);
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AFpsCharacter::InteractionPressed);
	PlayerInputComponent->BindAction("Interaction", IE_Released, this, &AFpsCharacter::InteractionReleased);

	// Switch weapons
	PlayerInputComponent->BindAction<FHandsSwitchDelegate>("SwapHands1", IE_Pressed, this, &AFpsCharacter::SwapHandsPressed, 0);
	PlayerInputComponent->BindAction<FHandsSwitchDelegate>("SwapHands2", IE_Pressed, this, &AFpsCharacter::SwapHandsPressed, 1);
	PlayerInputComponent->BindAction<FHandsSwitchDelegate>("SwapHands3", IE_Pressed, this, &AFpsCharacter::SwapHandsPressed, 2);
	PlayerInputComponent->BindAction<FHandsSwitchDelegate>("SwapHands4", IE_Pressed, this, &AFpsCharacter::SwapHandsPressed, 3);
	PlayerInputComponent->BindAction<FHandsSwitchDelegate>("SwapHands5", IE_Pressed, this, &AFpsCharacter::SwapHandsPressed, 4);

	// UI
	PlayerInputComponent->BindAction("GunShop", IE_Pressed, this, &AFpsCharacter::GunShopPressed);

}

void AFpsCharacter::MoveForward(float Value)
{
	if (CharacterStatus != EFpsCharacterStatus::Alive) return;
	AddMovementInput(CameraYawJointComponent->GetForwardVector(), Value);
}

void AFpsCharacter::MoveRight(float Value)
{
	if (CharacterStatus != EFpsCharacterStatus::Alive) return;
	AddMovementInput(CameraYawJointComponent->GetRightVector(), Value);
}

void AFpsCharacter::AddControllerPitchInput(float Value)
{
	if (CharacterStatus == EFpsCharacterStatus::Dead ||
		CharacterStatus == EFpsCharacterStatus::Freeze) return;
	Super::AddControllerPitchInput(Value);
}

void AFpsCharacter::AddControllerYawInput(float Value)
{
	if (CharacterStatus == EFpsCharacterStatus::Dead ||
		CharacterStatus == EFpsCharacterStatus::Freeze) return;
	Super::AddControllerYawInput(Value);
}

void AFpsCharacter::Jump()
{
	if (CharacterStatus != EFpsCharacterStatus::Alive) return;
	Super::Jump();
}

void AFpsCharacter::CrouchPressed()
{
	if (CharacterStatus != EFpsCharacterStatus::Alive) return;
	Crouch();
}

void AFpsCharacter::CrouchReleased()
{
	UnCrouch();
}

void AFpsCharacter::ActionPressed()
{
	for (IFpsCharacterEvent* EventObserver : EventObservers)
	{
		EventObserver->OnActionPressed();
	}
}

void AFpsCharacter::ActionReleased()
{
	for (IFpsCharacterEvent* EventObserver : EventObservers)
	{
		EventObserver->OnActionReleased();
	}
}

void AFpsCharacter::SubactionPressed()
{

	for (IFpsCharacterEvent* EventObserver : EventObservers)
	{
		EventObserver->OnSubactionPressed();
	}
}

void AFpsCharacter::SubactionReleased()
{
	for (IFpsCharacterEvent* EventObserver : EventObservers)
	{
		EventObserver->OnSubactionReleased();
	}
}

void AFpsCharacter::ReloadPressed()
{
	for (IFpsCharacterEvent* EventObserver : EventObservers)
	{
		EventObserver->OnReloadPressed();
	}
}

void AFpsCharacter::DropWeaponPressed()
{
	if (CharacterStatus == EFpsCharacterStatus::Dead ||
		CharacterStatus == EFpsCharacterStatus::Freeze ||
		!IsValid(Hands)) return;
	ServerRpcDropWeapon();
}

void AFpsCharacter::InteractionPressed()
{
	if (CharacterStatus == EFpsCharacterStatus::Dead ||
		!IsValid(InteractiveTarget))
		return;
	InteractiveTarget->OnInteractWith(this);
}

void AFpsCharacter::InteractionReleased()
{
	if (CharacterStatus == EFpsCharacterStatus::Dead ||
		!IsValid(InteractiveTarget))
		return;
	InteractiveTarget->OnInteractionStop(this);
}

void AFpsCharacter::SwapHandsPressed(int Index)
{
	ServerRpcSwapHandsTo(Index);
}

void AFpsCharacter::GunShopPressed()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Log, TEXT("PlayerController is invalid"));
		return;
	}

	AFpsCharacterHud* FpsHud = Cast<AFpsCharacterHud>(PlayerController->GetHUD());
	if (!IsValid(FpsHud))
	{
		UE_LOG(LogTemp, Log, TEXT("HUD is invalid"));
		return;
	}
	if (FpsHud->IsOpenGunShop())
	{
		FpsHud->CloseGunShop();
	}
	else
	{
		FpsHud->OpenGunShop();
	}
}

void AFpsCharacter::AddObserver(IFpsCharacterEvent* Observer)
{
	EventObservers.Add(Observer);
}

void AFpsCharacter::RemoveObserver(IFpsCharacterEvent* Observer)
{
	EventObservers.Remove(Observer);
}

void AFpsCharacter::OnPossessed()
{
	Respawn();
	InitializeGunShop();
	
}

void AFpsCharacter::OnPlayerFull()
{
	UE_LOG(LogTemp, Log, TEXT("AFpsCharacter::OnPlayerFull"));

	AWeaponBase* PrimaryWeapon = Cast<AWeaponBase>(Hands);
	if (IsValid(PrimaryWeapon))
	{
		PrimaryWeapon->StopAction();
		PrimaryWeapon->StopSubaction();
	}

	SetCharacterStatus(EFpsCharacterStatus::Freeze);
	ClientRpcSetAlertTextOnHud("Game is going to begin soon.");
}

void AFpsCharacter::OnRoundReady()
{
	Respawn();
	SetCharacterStatus(EFpsCharacterStatus::Stopped);
	ClientRpcSetAlertTextOnHud("Game Ready");
}

void AFpsCharacter::OnRoundStart()
{
	SetCharacterStatus(EFpsCharacterStatus::Alive);
	ClientRpcSetAlertTextOnHud("Round Start");

	const float TextDuration = 2.0f;
	GetWorldTimerManager().SetTimer(RoundTextTimerHandle, this, &AFpsCharacter::OnTextDurationFinished, TextDuration, false, TextDuration);
}

void AFpsCharacter::OnTextDurationFinished()
{
	ClientRpcSetAlertTextOnHud("");
}

void AFpsCharacter::OnRoundEnd()
{
	SetCharacterStatus(EFpsCharacterStatus::Freeze);
	ClientRpcSetAlertTextOnHud("Round End");
}

bool AFpsCharacter::ServerRpcPickUp_Validate(APickupableActor* PickupableActor)
{
	if (PickupableActor == nullptr) return false;
	return true;
}

void AFpsCharacter::ServerRpcPickUp_Implementation(APickupableActor* PickupableActor)
{
	AHands* HandsInstance = PickupableActor->GetHandsInstance();
	if (!IsValid(HandsInstance))
	{
		TSubclassOf<AHands> HandsSubclass = PickupableActor->GetHandsSubclass();
		if (!IsValid(HandsSubclass)) return;

		HandsInstance = GetWorld()->SpawnActor<AWeaponBase>(HandsSubclass, FVector(0, 0, 0), FRotator::ZeroRotator);
	}

	Acquire(HandsInstance, HandsInstance->GetHandsIndex());
	PickupableActor->Destroy();
}

void AFpsCharacter::ServerRpcDropWeapon_Implementation()
{
	DropWeapon();
}

bool AFpsCharacter::MulticastRpcSetActorRotation_Validate(FRotator Rotator)
{
	return true;
}

void AFpsCharacter::MulticastRpcSetActorRotation_Implementation(FRotator Rotator)
{
	SetActorRotation(Rotator);
}

bool AFpsCharacter::ClientRpcSetAlertTextOnHud_Validate(const FString& Text)
{
	return true;
}

void AFpsCharacter::ClientRpcSetAlertTextOnHud_Implementation(const FString& Text)
{
	SetAlertTextOnHud(Text);
}

void AFpsCharacter::MulticastRpcKnockOutBodyMesh_Implementation() 
{
	KnockOutBodyMesh();
}

bool AFpsCharacter::ServerRpcSetInteractiveTarget_Validate(AInteractiveActor* Actor)
{
	return true;
}

void AFpsCharacter::ServerRpcSetInteractiveTarget_Implementation(AInteractiveActor* Actor)
{
	SetInteractiveTarget(Actor);
}

bool AFpsCharacter::ServerRpcSwapHandsTo_Validate(int Index)
{
	if (Index < 0 || 5 < Index) return false;
	return true;
}

void AFpsCharacter::ServerRpcSwapHandsTo_Implementation(int Index)
{
	SwapHandsTo(Index);
}

void AFpsCharacter::OnRep_InitializePrimaryWeapon()
{
	AWeaponBase* PrimaryWeapon = Cast<AWeaponBase>(Hands);
	if (!IsValid(PrimaryWeapon)) return;
	PrimaryWeapon->Initialize(this);
}

float AFpsCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Log, TEXT("FPSCharacter TakeDamage"));
	if (CharacterStatus == EFpsCharacterStatus::Dead) return 0.f;

	float ReducedDamageByArmor = Damage * 0.5f;
	ReducedDamageByArmor = (ReducedDamageByArmor > Armor) ? Armor : ReducedDamageByArmor;
	Armor -= ReducedDamageByArmor;
	Health -= Damage - ReducedDamageByArmor;

	if (Health <= 0)
	{
		Die();
		const float RespawnDelay = 2.f;
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AFpsCharacter::Respawn, RespawnDelay, false, RespawnDelay);
	}

	return Damage;
}

void AFpsCharacter::Die()
{
	SetCharacterStatus(EFpsCharacterStatus::Dead);
	MulticastRpcKnockOutBodyMesh();
	ServerRpcDropWeapon();
}

void AFpsCharacter::Respawn()
{
	UE_LOG(LogTemp, Log, TEXT("Respawn"));
	WakeUpBodyMesh();
	InitializeGameplayVariable();
	SetActorTransform(SpawnTransform);
}

void AFpsCharacter::KnockOutBodyMesh()
{
	BodyMeshComponent->SetSimulatePhysics(true);
}

void AFpsCharacter::WakeUpBodyMesh()
{
	BodyMeshComponent->SetSimulatePhysics(false);
	BodyMeshComponent->SetRelativeLocation(DefaultLocationOfBodyMeshComponent);
	BodyMeshComponent->SetRelativeRotation(DefaultRotatorOfBodyMeshComponent);
}

void AFpsCharacter::Equip(AHands* HandsInstance)
{
	Hands = HandsInstance;
	Hands->AttachToComponent(CameraComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
	Hands->Initialize(this);

	HandsModelForBody = Hands->CreateHandsModelForBody();
	if (!IsValid(HandsModelForBody)) return;
	HandsModelForBody->AttachToComponent(BodyMeshComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), HandsModelForBody->GetAttachingGripPointName());
	HandsModelForBody->Initialize(this);

	for (IFpsCharacterEvent* EventObserver : EventObservers)
	{
		EventObserver->OnEquipHands(Hands);
	}
}

AHands* AFpsCharacter::Unequip()
{
	if (!IsValid(Hands))
	{
		UE_LOG(LogTemp, Log, TEXT("AFpsCharacter::Unequip Hands is already invalid"));
		return nullptr;
	}

	Hands->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	for (IFpsCharacterEvent* EventObserver : EventObservers)
	{
		EventObserver->OnUnequipHands(Hands);
	}

	// Unequip WeaponModelForThirdPerson
	if (!IsValid(HandsModelForBody)) 
	{
		UE_LOG(LogTemp, Log, TEXT("AFpsCharacter::Unequip HandsModelForBody is invalid"))
		return Hands;
	}
	HandsModelForBody->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	HandsModelForBody->Destroy();

	return Hands;
}

void AFpsCharacter::DropWeapon()
{
	if (!IsValid(Hands))
	{
		UE_LOG(LogTemp, Log, TEXT("DropWeapon() : Hands is invalid"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("DropWeapon() : dropped!"));

	//Get Player direction for adding impulse to PickUpWeapon.
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Log, TEXT("ServerRpcDropWeapon() : PlayerController is invalid"));
		return;
	}
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	PlayerController->GetPlayerViewPoint(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);

	//Unequip and Spawn APickupableActor
	APickupableActor* PickupableActor = Hands->CreatePickupableActor();
	if (!IsValid(PickupableActor))
	{
		UE_LOG(LogTemp, Log, TEXT("PickupableActor is invalid"));
		return;
	}
	AHands* HandsInstance = Unequip();
	PickupableActor->SetHandsInstance(HandsInstance);

	//Add impulse to viewpoint direction
	UStaticMeshComponent* PickupableActorMesh = PickupableActor->GetMesh();
	const float ImpulsePower = 300.f;
	PickupableActorMesh->AddImpulse(PlayerViewPointRotation.Vector() * PickupableActorMesh->GetMass() * ImpulsePower);
}

void AFpsCharacter::Acquire(AHands* HandsInstance, int HandsIndex)
{
	UE_LOG(LogTemp, Log, TEXT("AFpsCharacter::Acquire(%s, %d)"), *HandsInstance->GetName(), HandsIndex);
	if (!IsValid(Inventory[HandsIndex]))
	{
		Unequip();
	}
	else
	{
		if (HandsIndex != CurrentHandsIndex)
		{
			SwapHandsTo(HandsIndex);
		}
		DropWeapon();
	}
	Equip(HandsInstance);
	SetHandsAtInventory(HandsInstance, HandsIndex);
}

void AFpsCharacter::SwapHandsTo(int Index)
{
	if (!IsValid(Inventory[Index]))
	{
		UE_LOG(LogTemp, Log, TEXT("Inventory[%d] is invalid"), Index);
		return;
	}
	PreviousHandsIndex = CurrentHandsIndex;
	CurrentHandsIndex = Index;
	Unequip();
	Equip(Inventory[Index]);
}

void AFpsCharacter::SwapHandsToPrevious()
{
	if (!IsValid(Inventory[PreviousHandsIndex]) || PreviousHandsIndex == CurrentHandsIndex) 
	{
		UE_LOG(LogTemp, Log, TEXT("Inventory[PreviousHandsIndex] is invalid or PerviousHandsIndex is same with CurrnetHandsIndex"));
		Unequip();
		return;
	}

	SwapHandsTo(PreviousHandsIndex);
}

void AFpsCharacter::SetAlertTextOnHud(FString Text)
{
	AFpsPlayerController* PlayerController = Cast<AFpsPlayerController>(GetController());
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Log, TEXT("AFpsCharacter::SetAlertTextOnHud / PlayerController is invalid"));
		return;
	}
	AFpsCharacterHud* CharacterHud = Cast<AFpsCharacterHud>(PlayerController->GetHUD());
	if (!IsValid(CharacterHud))
	{
		UE_LOG(LogTemp, Log, TEXT("AFpsCharacter::SetAlertTextOnHud / CharacterHud is invalid"));
		return;
	}
	UFpsCharacterWidget* CharacterWidget = Cast<UFpsCharacterWidget>(CharacterHud->GetDefaultWidget());
	if (!IsValid(CharacterWidget))
	{
		UE_LOG(LogTemp, Log, TEXT("AFpsCharacter::SetAlertTextOnHud / CharacterWidget is invalid"));
		return;
	}
	CharacterWidget->SetAlertText(Text);
}

float AFpsCharacter::GetHealth()
{
	return Health;
}

float AFpsCharacter::GetArmor()
{
	return Armor;
}

UCameraComponent* AFpsCharacter::GetCameraComponent()
{
	return CameraComponent;
}

AHands* AFpsCharacter::GetHands()
{
	return Hands;
}

USkeletalMeshComponent* AFpsCharacter::GetBodyMeshComponent()
{
	return BodyMeshComponent;
}

float AFpsCharacter::GetAimPtich()
{
	return BodyMeshAimPitch;
}

float AFpsCharacter::GetAimYaw()
{
	return BodyMeshAimYaw;
}

TSubclassOf<AHUD> AFpsCharacter::GetHudSubclass()
{
	return HudSubclass;
}

AGunShop* AFpsCharacter::GetGunShop()
{
	return GunShop;
}

AHandsModelForBody* AFpsCharacter::GetHandsModelForBody()
{
	return HandsModelForBody;
}

void AFpsCharacter::SetSpawnTransform(FTransform Transform) 
{
	SpawnTransform = Transform;
}

void AFpsCharacter::SetCharacterStatus(EFpsCharacterStatus Status) 
{
	CharacterStatus = Status;
}

void AFpsCharacter::SetInteractiveTarget(AInteractiveActor* Actor)
{
	InteractiveTarget = Actor;
}

void AFpsCharacter::SetHandsAtInventory(AHands* HandsInstance, int Index)
{
	Inventory[Index] = HandsInstance;
}

void AFpsCharacter::EnablePlanting(bool IsPlantable)
{
	Plantable = IsPlantable;
}

bool AFpsCharacter::IsPlantable()
{
	return Plantable;
}