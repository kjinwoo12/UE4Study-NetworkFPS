// Fill out your copyright notice in the Description page of Project Settings.

#include "FpsCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "../Weapons/WeaponBase.h"
#include "../Weapons/PickUpWeapon.h"
#include "../Weapons/WeaponModelForBody.h"
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
	InitializeHandsMesh();
	InitializeBodyMesh();
	InitializeGameplayVariable();
}

void AFpsCharacter::InitializeCollisionComponent()
{
	UCapsuleComponent* CharacterCapsuleComponent = GetCapsuleComponent();
	CharacterCapsuleComponent->SetCapsuleHalfHeight(94.0f);
	CharacterCapsuleComponent->SetCapsuleRadius(42.0f);
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

void AFpsCharacter::InitializeHandsMesh()
{
	if (!IsValid(CameraComponent))
	{
		UE_LOG(LogTemp, Log, TEXT("Failed initializeHandsMesh() : No CameraComponent"));
		return;
	}
	HandsMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandsMesh"));
	HandsMeshComponent->SetOnlyOwnerSee(true);
	HandsMeshComponent->SetupAttachment(CameraComponent);
	HandsMeshComponent->bCastDynamicShadow = false;
	HandsMeshComponent->CastShadow = false;
	HandsMeshComponent->SetRelativeLocation(DefaultLocationOfHandsMeshComponent);
	HandsMeshComponent->SetRelativeRotation(DefaultRotatorOfHandsMeshComponent);
}

void AFpsCharacter::InitializeBodyMesh()
{
	BodyMeshComponent = GetMesh();
	BodyMeshComponent->SetIsReplicated(true);
	BodyMeshComponent->SetOwnerNoSee(true);
	BodyMeshComponent->SetRelativeLocation(DefaultLocationOfBodyMeshComponent);
	BodyMeshComponent->SetRelativeRotation(DefaultRotatorOfBodyMeshComponent);
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
}

void AFpsCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFpsCharacter, Health);
	DOREPLIFETIME(AFpsCharacter, Armor);
	DOREPLIFETIME(AFpsCharacter, PickableWeapon);
	DOREPLIFETIME(AFpsCharacter, PrimaryWeapon);
	DOREPLIFETIME(AFpsCharacter, WeaponModelForBody);
	DOREPLIFETIME(AFpsCharacter, CharacterStatus);
	DOREPLIFETIME(AFpsCharacter, AimPitch);
	DOREPLIFETIME(AFpsCharacter, AimYaw);
	DOREPLIFETIME(AFpsCharacter, GunShop);
}

// Called every frame
void AFpsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateAim(DeltaTime);
	UpdateActorDirection(DeltaTime);
	UpdateCameraRotation();
	UpdateCrosshair();
}

void AFpsCharacter::UpdateCrosshair()
{
	if (GetNetMode() == NM_DedicatedServer) return;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!IsValid(PlayerController) || !IsValid(PrimaryWeapon)) return;

	AFpsCharacterHud* FpsHud = Cast<AFpsCharacterHud>(PlayerController->GetHUD());
	if (!IsValid(FpsHud)) return;

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

void AFpsCharacter::UpdateInteractiveActor(float DeltaTime) 
{
	APlayerController* PlayerController = GetController<APlayerController>();
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
		ECollisionChannel::ECC_GameTraceChannel1,
		LineTraceCollisionQueryParams
	);
	if (!IsHit)	return;

	AInteractiveActor *InteractiveActor = Cast<AInteractiveActor>(HitResult.Actor);
	if (!IsValid(InteractiveActor)) return;

	InteractiveTarget = InteractiveActor;
	InteractiveTarget->OnTartgetedBy(this);
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

	// Actions
	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AFpsCharacter::ActionPressed);
	PlayerInputComponent->BindAction("Action", IE_Released, this, &AFpsCharacter::ActionReleased);
	PlayerInputComponent->BindAction("Subaction", IE_Pressed, this, &AFpsCharacter::SubactionPressed);
	PlayerInputComponent->BindAction("Subaction", IE_Released, this, &AFpsCharacter::SubactionReleased);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFpsCharacter::ReloadPressed);
	PlayerInputComponent->BindAction("PickUpWeapon", IE_Pressed, this, &AFpsCharacter::PickUpWeaponPressed);
	PlayerInputComponent->BindAction("DropWeapon", IE_Pressed, this, &AFpsCharacter::DropWeaponPressed);
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AFpsCharacter::InteractionPressed);
	PlayerInputComponent->BindAction("Interaction", IE_Released, this, &AFpsCharacter::InteractionReleased);

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

void AFpsCharacter::ActionPressed()
{
	if (CharacterStatus != EFpsCharacterStatus::Alive ||
		!IsValid(PrimaryWeapon)) return;
	ServerRpcStartAction();
}

void AFpsCharacter::ActionReleased()
{
	if (CharacterStatus != EFpsCharacterStatus::Alive ||
		!IsValid(PrimaryWeapon)) return;
	ServerRpcStopAction();
}

void AFpsCharacter::SubactionPressed()
{
	if (CharacterStatus != EFpsCharacterStatus::Alive ||
		!IsValid(PrimaryWeapon)) return;
	ServerRpcStartSubaction();
}

void AFpsCharacter::SubactionReleased()
{
	if (CharacterStatus != EFpsCharacterStatus::Alive ||
		!IsValid(PrimaryWeapon)) return;
	ServerRpcStopSubaction();
}

void AFpsCharacter::ReloadPressed()
{
	if (CharacterStatus != EFpsCharacterStatus::Alive ||
		!IsValid(PrimaryWeapon)) return;
	ServerRpcStartReload();
}

void AFpsCharacter::PickUpWeaponPressed()
{
	if (CharacterStatus == EFpsCharacterStatus::Dead ||
		CharacterStatus == EFpsCharacterStatus::Freeze ||
		IsValid(PrimaryWeapon)) return;
	PickUpWeapon();
}

void AFpsCharacter::DropWeaponPressed()
{
	if (CharacterStatus == EFpsCharacterStatus::Dead ||
		CharacterStatus == EFpsCharacterStatus::Freeze ||
		!IsValid(PrimaryWeapon)) return;
	ServerRpcStopAction();
	ServerRpcStopSubaction();
	DropWeapon();
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
	InteractiveTarget->OnInteractWith(this);
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
	if (!IsValid(PrimaryWeapon))
	{
		UE_LOG(LogTemp, Log, TEXT("AFpsCharacter::ServerRpcStartAction_Implementation = PrimaryWeapon NULL"));
		return;
	}
	PrimaryWeapon->StartAction();
}

void AFpsCharacter::ServerRpcStopAction_Implementation()
{
	if (!IsValid(PrimaryWeapon)) return;
	PrimaryWeapon->StopAction();
}

void AFpsCharacter::ServerRpcStartSubaction_Implementation()
{
	if (!IsValid(PrimaryWeapon)) return;
	PrimaryWeapon->StartSubaction();
}

void AFpsCharacter::ServerRpcStopSubaction_Implementation()
{
	if (!IsValid(PrimaryWeapon)) return;
	PrimaryWeapon->StopSubaction();
}

void AFpsCharacter::ServerRpcStartReload_Implementation()
{
	if (!IsValid(PrimaryWeapon)) return;
	PrimaryWeapon->StartReload();
}

void AFpsCharacter::ServerRpcPickUpWeapon_Implementation()
{
	if (IsValid(PrimaryWeapon) || !IsValid(PickableWeapon)) return;

	AWeaponBase* WeaponInstance = PickableWeapon->GetWeaponInstance();
	if (!IsValid(WeaponInstance))
	{
		TSubclassOf<AWeaponBase> WeaponBaseSubclass = PickableWeapon->GetWeaponBaseSubclass();
		if (WeaponBaseSubclass == nullptr) return;

		WeaponInstance = AWeaponBase::SpawnWeapon(GetWorld(), WeaponBaseSubclass);
	}
	EquipWeapon(WeaponInstance);

	PickableWeapon->Destroy();
	PickableWeapon = NULL;
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

void AFpsCharacter::OnRep_InitializePrimaryWeapon()
{
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
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Visibility);
	HandsMeshComponent->SetOwnerNoSee(true);

	MulticastRpcKnockOutBodyMesh();
	ServerRpcDropWeapon();
}

void AFpsCharacter::Respawn()
{
	UE_LOG(LogTemp, Log, TEXT("Respawn"));

	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	HandsMeshComponent->SetOwnerNoSee(false);

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

void AFpsCharacter::EquipWeapon(AWeaponBase* WeaponBase)
{
	// Attach AWeaponBase Actor for first-person view
	PrimaryWeapon = WeaponBase;
	FName AttachingGripPointName = PrimaryWeapon->GetAttachingGripPointName();
	PrimaryWeapon->AttachToComponent(HandsMeshComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), AttachingGripPointName);
	PrimaryWeapon->Initialize(this);

	// Attach AWeaponModelForBody for third-person view
	WeaponModelForBody = PrimaryWeapon->SpawnWeaponModelForBodyActor();
	if (!IsValid(WeaponModelForBody)) return;
	WeaponModelForBody->SetOwner(this);
	WeaponModelForBody->AttachToComponent(BodyMeshComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), AttachingGripPointName);
}

AWeaponBase* AFpsCharacter::UnEquipWeapon()
{
	if (!IsValid(PrimaryWeapon)) return NULL;

	// UnEquip PrimaryWeapon
	AWeaponBase* WeaponInstance = PrimaryWeapon;
	PrimaryWeapon = nullptr;
	WeaponInstance->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	WeaponInstance->SetOwner(NULL);
	WeaponInstance->OnUnEquipped();

	// UnEquip WeaponModelForThirdPerson
	if (!IsValid(WeaponModelForBody)) return WeaponInstance;
	WeaponModelForBody->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	WeaponModelForBody->Destroy();

	return WeaponInstance;
}

void AFpsCharacter::DropWeapon()
{
	if (!IsValid(PrimaryWeapon))
	{
		UE_LOG(LogTemp, Log, TEXT("DropWeapon() : PrimaryWeapon is invalid"));
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

	//Unequip and Spawn APickUpWeapon
	AWeaponBase* WeaponInstance = UnEquipWeapon();
	APickUpWeapon* PickUpWeapon = WeaponInstance->SpawnPickUpWeaponActor();
	PickUpWeapon->SetWeaponInstance(WeaponInstance);

	//Add impulse to viewpoint direction
	UStaticMeshComponent* WeaponMesh = PickUpWeapon->GetWeaponMesh();
	const float ImpulsePower = 300.f;
	WeaponMesh->AddImpulse(PlayerViewPointRotation.Vector() * WeaponMesh->GetMass() * ImpulsePower);
}

void AFpsCharacter::PickUpWeapon()
{
	UE_LOG(LogTemp, Log, TEXT("PickUpWeapon()"));
	ServerRpcPickUpWeapon();
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

AWeaponBase* AFpsCharacter::GetPrimaryWeapon()
{
	return PrimaryWeapon;
}

USkeletalMeshComponent* AFpsCharacter::GetHandsMeshComponent()
{
	return HandsMeshComponent;
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

void AFpsCharacter::SetPickableWeapon(APickUpWeapon* Instance)
{
	PickableWeapon = Instance;
}

void AFpsCharacter::SetSpawnTransform(FTransform Transform) 
{
	SpawnTransform = Transform;
}

void AFpsCharacter::SetCharacterStatus(EFpsCharacterStatus Status) 
{
	CharacterStatus = Status;
}