// Fill out your copyright notice in the Description page of Project Settings.

#include "FpsCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "../Weapons/Hands.h"
#include "../Weapons/WeaponBase.h"
#include "../Weapons/PickupableActor.h"
#include "../Weapons/HandsModelForBody.h"
#include "../Ui/FpsCharacterHud.h"
#include "../Ui/FpsCharacterWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "../PlayerController/FpsPlayerController.h"
#include "../PlayerController/FpsPlayerState.h"
#include "GunShop.h"
#include "InteractiveActor.h"

// Sets default values
AFpsCharacter::AFpsCharacter()
{
	bReplicates = true;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitializeCollisionComponent();
	InitializeMovementComponent();
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

void AFpsCharacter::InitializeCamera()
{
	// Camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCameraComponent"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(0.f, 0.f, BaseEyeHeight));
	CameraComponent->bUsePawnControlRotation = true;
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
	AimPitch = 0;
	AimYaw = 0;
	GunShop = nullptr;
}

void AFpsCharacter::InitializeGunShop()
{
	GunShop = GetWorld()->SpawnActor<AGunShop>();
	if (IsValid(GunShop)) 
	{
		UE_LOG(LogTemp, Log, TEXT("GunShop is spawned"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("GunShop is invalid"));
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
	DOREPLIFETIME(AFpsCharacter, AimPitch);
	DOREPLIFETIME(AFpsCharacter, AimYaw);
	DOREPLIFETIME(AFpsCharacter, GunShop);
	DOREPLIFETIME(AFpsCharacter, CurrentHandsIndex);
	DOREPLIFETIME(AFpsCharacter, Inventory);
}

// Called every frame
void AFpsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateAim(DeltaTime);
	UpdateActorDirection(DeltaTime);
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
	if (!IsValid(PrimaryWeapon)) {
		FpsHud->SetCrosshairCenterOffset(0.f);
		return;
	}

	const float CharacterSpeedOffset = GetVelocity().Size() / PrimaryWeapon->GetMovementStability();
	const int JumpingOffset = (MovementComponent->IsFalling()) ? 30 : 0;
	const float CrosshairCenterOffset = CharacterSpeedOffset + JumpingOffset;
	FpsHud->SetCrosshairCenterOffset(CrosshairCenterOffset);
}

void AFpsCharacter::ClientRpcUpdateCameraRotationToServer_Implementation()
{
	ServerRpcSetCameraRotation(CameraComponent->GetComponentToWorld().GetRotation());
}

bool AFpsCharacter::ServerRpcSetCameraRotation_Validate(FQuat CameraRotation)
{
	return true;
}

void AFpsCharacter::ServerRpcSetCameraRotation_Implementation(FQuat CameraRotation)
{
	CameraComponent->SetWorldRotation(CameraRotation);
}

void AFpsCharacter::UpdateAim(float DeltaTime) 
{
	if (GetNetMode() == NM_Client) return;

	FRotator AimRotator = FRotator(AimPitch, AimYaw, 0);
	FRotator ControlRotation = GetControlRotation();
	FRotator ActorRotation = GetActorRotation();
	AimRotator = UKismetMathLibrary::RInterpTo(
		AimRotator,
		UKismetMathLibrary::NormalizedDeltaRotator(ControlRotation, ActorRotation),
		DeltaTime,
		0
	);
	AimPitch = UKismetMathLibrary::ClampAngle(AimRotator.Pitch, -90, 90);
	AimYaw = UKismetMathLibrary::ClampAngle(AimRotator.Yaw, -90, 90);
}

void AFpsCharacter::UpdateActorDirection(float DeltaTime)
{
	if (GetNetMode() == NM_Client) return;

	// if the character is moving, or the Controller's aim direction is over 90 degree,
	// then make body of the character to follow the aim direction.
	FRotator AimRotator = FRotator(AimPitch, AimYaw, 0);
	FRotator ControlRotation = GetControlRotation();
	FRotator ActorRotation = GetActorRotation();
	float CharacterSpeed = 0;
	FVector BodyDirection;
	GetVelocity().ToDirectionAndLength(BodyDirection, CharacterSpeed);
	if (CharacterSpeed > 0)
	{ 
		MulticastRpcSetActorRotation(FRotator(0, ControlRotation.Yaw, 0));
	}
	else if (AimRotator.Yaw < -90 || 90 < AimRotator.Yaw)
	{
		MulticastRpcSetActorRotation(FRotator(0, ActorRotation.Yaw + AimRotator.Yaw - AimYaw, 0));
	}
}

void AFpsCharacter::UpdateCameraRotation()
{
	if (GetNetMode() != ENetMode::NM_DedicatedServer) return;
	ClientRpcUpdateCameraRotationToServer();
}

void AFpsCharacter::UpdateInteractiveTarget(float DeltaTime) 
{
	if (GetNetMode() == NM_DedicatedServer) return;
	APlayerController* PlayerController = GetController<APlayerController>();
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Log, TEXT("AFpsCharacter::UpdateInteractiveTarget PlayerController is invalid"));
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
	PlayerInputComponent->BindAction<FHandsActionDelegate>("Action", IE_Pressed, this, &AFpsCharacter::OnActionEvent, FName("Action"), IE_Pressed);
	PlayerInputComponent->BindAction<FHandsActionDelegate>("Action", IE_Released, this, &AFpsCharacter::OnActionEvent, FName("Action"), IE_Released);
	PlayerInputComponent->BindAction<FHandsActionDelegate>("Subaction", IE_Pressed, this, &AFpsCharacter::OnActionEvent, FName("Subaction"), IE_Pressed);
	PlayerInputComponent->BindAction<FHandsActionDelegate>("Subaction", IE_Released, this, &AFpsCharacter::OnActionEvent, FName("Subaction"), IE_Released);
	PlayerInputComponent->BindAction<FHandsActionDelegate>("Reload", IE_Pressed, this, &AFpsCharacter::OnActionEvent, FName("Reload"), IE_Pressed);
	
	// Interaction
	PlayerInputComponent->BindAction("DropWeapon", IE_Pressed, this, &AFpsCharacter::DropWeaponPressed);
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AFpsCharacter::InteractionPressed);
	PlayerInputComponent->BindAction("Interaction", IE_Released, this, &AFpsCharacter::InteractionReleased);

	// Switch weapons
	PlayerInputComponent->BindAction<FHandsSwitchDelegate>("WeaponSwitch1", IE_Pressed, this, &AFpsCharacter::WeaponSwitchPressed, 0);
	PlayerInputComponent->BindAction<FHandsSwitchDelegate>("WeaponSwitch2", IE_Pressed, this, &AFpsCharacter::WeaponSwitchPressed, 1);
	PlayerInputComponent->BindAction<FHandsSwitchDelegate>("WeaponSwitch3", IE_Pressed, this, &AFpsCharacter::WeaponSwitchPressed, 2);
	PlayerInputComponent->BindAction<FHandsSwitchDelegate>("WeaponSwitch4", IE_Pressed, this, &AFpsCharacter::WeaponSwitchPressed, 3);
	PlayerInputComponent->BindAction<FHandsSwitchDelegate>("WeaponSwitch5", IE_Pressed, this, &AFpsCharacter::WeaponSwitchPressed, 4);

	// UI
	PlayerInputComponent->BindAction("GunShop", IE_Pressed, this, &AFpsCharacter::GunShopPressed);

}

void AFpsCharacter::MoveForward(float Value)
{
	if (CharacterStatus != EFpsCharacterStatus::Alive) return;
	AddMovementInput(GetActorForwardVector(), Value);
}

void AFpsCharacter::MoveRight(float Value)
{
	if (CharacterStatus != EFpsCharacterStatus::Alive) return;
	AddMovementInput(GetActorRightVector(), Value);
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

void AFpsCharacter::OnActionEvent(FName ActionName, EInputEvent KeyEvent)
{
	if (CharacterStatus != EFpsCharacterStatus::Alive ||
		!IsValid(Hands)) return;
	Hands->OnActionEvent(ActionName, KeyEvent);
}

void AFpsCharacter::DropWeaponPressed()
{
	if (CharacterStatus == EFpsCharacterStatus::Dead ||
		CharacterStatus == EFpsCharacterStatus::Freeze ||
		!IsValid(Hands)) return;
	ServerRpcStopAction();
	ServerRpcStopSubaction();
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

void AFpsCharacter::WeaponSwitchPressed(int Index)
{
	ServerRpcWeaponSwitch(Index);
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
}

void AFpsCharacter::OnRoundEnd()
{
	SetCharacterStatus(EFpsCharacterStatus::Freeze);
	ClientRpcSetAlertTextOnHud("Round End");
}

void AFpsCharacter::ServerRpcStartAction_Implementation()
{
	AWeaponBase* PrimaryWeapon = Cast<AWeaponBase>(Hands);
	if (!IsValid(PrimaryWeapon))
	{
		UE_LOG(LogTemp, Log, TEXT("AFpsCharacter::ServerRpcStartAction_Implementation = PrimaryWeapon NULL"));
		return;
	}
	PrimaryWeapon->StartAction();
}

void AFpsCharacter::ServerRpcStopAction_Implementation()
{
	AWeaponBase* PrimaryWeapon = Cast<AWeaponBase>(Hands);
	if (!IsValid(PrimaryWeapon)) return;
	PrimaryWeapon->StopAction();
}

void AFpsCharacter::ServerRpcStartSubaction_Implementation()
{
	AWeaponBase* PrimaryWeapon = Cast<AWeaponBase>(Hands);
	if (!IsValid(PrimaryWeapon)) return;
	PrimaryWeapon->StartSubaction();
}

void AFpsCharacter::ServerRpcStopSubaction_Implementation()
{
	AWeaponBase* PrimaryWeapon = Cast<AWeaponBase>(Hands);
	if (!IsValid(PrimaryWeapon)) return;
	PrimaryWeapon->StopSubaction();
}

void AFpsCharacter::ServerRpcStartReload_Implementation()
{
	AWeaponBase* PrimaryWeapon = Cast<AWeaponBase>(Hands);
	if (!IsValid(PrimaryWeapon)) return;
	PrimaryWeapon->StartReload();
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
		TSubclassOf<AWeaponBase> WeaponBaseSubclass = PickupableActor->GetWeaponBaseSubclass();
		if (!IsValid(WeaponBaseSubclass)) return;

		HandsInstance = AWeaponBase::SpawnWeapon(GetWorld(), WeaponBaseSubclass);
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

bool AFpsCharacter::ServerRpcWeaponSwitch_Validate(int Index)
{
	if (Index < 0 || 5 < Index) return false;
	return true;
}

void AFpsCharacter::ServerRpcWeaponSwitch_Implementation(int Index)
{
	WeaponSwitch(Index);
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
	HandsModelForBody->SetOwner(this);
	HandsModelForBody->AttachToComponent(BodyMeshComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), Hands->GetAttachingGripPointName());
}

AHands* AFpsCharacter::UnEquip()
{
	if (!IsValid(Hands)) return nullptr;

	// UnEquip PrimaryWeapon
	AWeaponBase* WeaponInstance = Cast<AWeaponBase>(Hands);
	if (!IsValid(Hands)) return nullptr;

	Hands = nullptr;
	WeaponInstance->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	WeaponInstance->OnUnEquipped();

	// UnEquip WeaponModelForThirdPerson
	if (!IsValid(HandsModelForBody)) return WeaponInstance;
	HandsModelForBody->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	HandsModelForBody->Destroy();

	return WeaponInstance;
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
	AHands* HandsInstance = UnEquip();
	APickupableActor* PickupableActor = HandsInstance->CreatePickupableActor();
	if (!IsValid(PickupableActor))
	{
		UE_LOG(LogTemp, Log, TEXT("PickupableActor is invalid"));
		return;
	}
	PickupableActor->SetHandsInstance(HandsInstance);

	//Add impulse to viewpoint direction
	UStaticMeshComponent* PickupableActorMesh = PickupableActor->GetMesh();
	const float ImpulsePower = 300.f;
	PickupableActorMesh->AddImpulse(PlayerViewPointRotation.Vector() * PickupableActorMesh->GetMass() * ImpulsePower);
}

void AFpsCharacter::Acquire(AHands* HandsInstance, int HandsIndex)
{
	UE_LOG(LogTemp, Log, TEXT("AFpsChar7acter::Acquire(%s, %d)"), *HandsInstance->GetName(), HandsIndex);
	if (!IsValid(Inventory[HandsIndex]))
	{
		UnEquip();
	}
	else
	{
		if (HandsIndex != CurrentHandsIndex)
		{
			UnEquip();
			Equip(Inventory[HandsIndex]);
			CurrentHandsIndex = HandsIndex;
		}
		DropWeapon();
	}
	Equip(HandsInstance);
	SetHandsAtInventory(HandsInstance, HandsIndex);
}

void AFpsCharacter::WeaponSwitch(int Index)
{
	if (Inventory[Index] == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("WeaponInventory[%d] is Invalid"), Index);
		return;
	}
	CurrentHandsIndex = Index;
	UnEquip();
	Equip(Inventory[Index]);
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
	return AimPitch;
}

float AFpsCharacter::GetAimYaw()
{
	return AimYaw;
}

TSubclassOf<AHUD> AFpsCharacter::GetHudSubclass()
{
	return HudSubclass;
}

AGunShop* AFpsCharacter::GetGunShop()
{
	return GunShop;
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