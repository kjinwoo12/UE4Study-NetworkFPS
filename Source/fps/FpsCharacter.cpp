// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSCharacter.h"
#include "FPS.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "WeaponBase.h"
#include "PickUpWeapon.h"
#include "WeaponModelForBody.h"
#include "FPSHUD.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"

#include "DrawDebugHelpers.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
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

void AFPSCharacter::InitializeCollisionComponent()
{
	UCapsuleComponent* CharacterCapsuleComponent = GetCapsuleComponent();
	CharacterCapsuleComponent->SetCapsuleHalfHeight(94.0f);
	CharacterCapsuleComponent->SetCapsuleRadius(42.0f);
}

void AFPSCharacter::InitializeMovementComponent()
{
	MovementComponent = ACharacter::GetCharacterMovement();
	MovementComponent->bUseFlatBaseForFloorChecks = true;
	MovementComponent->MaxWalkSpeed = 400;
	MovementComponent->MaxWalkSpeedCrouched = 200;
	MovementComponent->NavAgentProps.bCanCrouch = true;
}

void AFPSCharacter::InitializeCamera()
{
	// Camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCameraComponent"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(0.f, 0.f, BaseEyeHeight));
	CameraComponent->bUsePawnControlRotation = true;
}

void AFPSCharacter::InitializeHandsMesh()
{
	if (CameraComponent == NULL)
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

void AFPSCharacter::InitializeBodyMesh()
{
	BodyMeshComponent = GetMesh();
	BodyMeshComponent->SetOwnerNoSee(true);
	BodyMeshComponent->SetRelativeLocation(DefaultLocationOfBodyMeshComponent);
	BodyMeshComponent->SetRelativeRotation(DefaultRotatorOfBodyMeshComponent);
}

void AFPSCharacter::InitializeGameplayVariable()
{
	MaxHealth = 100;
	Health = 100;
	MaxArmor = 100;
	Armor = 50;
	IsDead = false;
	AimPitch = 0;
	AimYaw = 0;
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("We are using FPSCharacter."));
	}

	HUD = Cast<AFPSHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (GetNetMode() == NM_ListenServer)
	{
		EquipWeapon(AWeaponBase::SpawnWeapon(GetWorld(), "Class'/Game/MyContent/Weapons/BP_HitScanWeapon_TestGun.BP_HitScanWeapon_TestGun_C'"));
	}
}

void AFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSCharacter, Health);
	DOREPLIFETIME(AFPSCharacter, Armor);
	DOREPLIFETIME(AFPSCharacter, PickableWeapon);
	DOREPLIFETIME(AFPSCharacter, PrimaryWeapon);
	DOREPLIFETIME(AFPSCharacter, WeaponModelForBody);
	DOREPLIFETIME(AFPSCharacter, AimPitch);
	DOREPLIFETIME(AFPSCharacter, AimYaw);
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ClientRPCTickCrosshair();
	UpdateActorDirectionByAim(DeltaTime);
}

void AFPSCharacter::ClientRPCTickCrosshair_Implementation()
{
	if (PrimaryWeapon == NULL || HUD == NULL) return;
	const float CharacterSpeedOffset = GetVelocity().Size() / PrimaryWeapon->GetMovementStability();
	const int JumpingOffset = (MovementComponent->IsFalling()) ? 30 : 0;
	const float CrosshairCenterOffset = CharacterSpeedOffset + JumpingOffset;
	HUD->SetCrosshairCenterOffset(CrosshairCenterOffset);
}

void AFPSCharacter::UpdateActorDirectionByAim(float DeltaTime)
{
	if (GetNetMode() != NM_ListenServer) return;

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

	// if the character is moving, or the Controller's aim direction is over 90 degree,
	// then make body of the character to follow the aim direction.
	float CharacterSpeed = 0;
	FVector BodyDirection;
	GetVelocity().ToDirectionAndLength(BodyDirection, CharacterSpeed);
	if (CharacterSpeed > 0)
	{ 
		MulticastRPCSetActorRotation(FRotator(0, ControlRotation.Yaw, 0));
	}
	else if (AimRotator.Yaw < -90 || 90 < AimRotator.Yaw)
	{
		MulticastRPCSetActorRotation(FRotator(0, ActorRotation.Yaw + AimRotator.Yaw - AimYaw, 0));
	}
}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::Jump);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AFPSCharacter::CrouchPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AFPSCharacter::CrouchReleased);

	// Actions
	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AFPSCharacter::ActionPressed);
	PlayerInputComponent->BindAction("Action", IE_Released, this, &AFPSCharacter::ActionReleased);
	PlayerInputComponent->BindAction("Subaction", IE_Pressed, this, &AFPSCharacter::SubactionPressed);
	PlayerInputComponent->BindAction("Subaction", IE_Released, this, &AFPSCharacter::SubactionReleased);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFPSCharacter::ReloadPressed);
	PlayerInputComponent->BindAction("PickUpWeapon", IE_Pressed, this, &AFPSCharacter::PickUpWeaponPressed);
	PlayerInputComponent->BindAction("DropWeapon", IE_Pressed, this, &AFPSCharacter::DropWeaponPressed);

	// UI
	PlayerInputComponent->BindAction("GunShop", IE_Pressed, this, &AFPSCharacter::GunShopPressed);
}

void AFPSCharacter::MoveForward(float Value)
{
	if (IsDead) return;
	AddMovementInput(GetActorForwardVector(), Value);
}

void AFPSCharacter::MoveRight(float Value)
{
	if (IsDead) return;
	AddMovementInput(GetActorRightVector(), Value);
}

void AFPSCharacter::AddControllerPitchInput(float Value)
{
	if (IsDead) return;
	Super::AddControllerPitchInput(Value);
}

void AFPSCharacter::AddControllerYawInput(float Value)
{
	if (IsDead) return;
	Super::AddControllerYawInput(Value);
}

void AFPSCharacter::Jump()
{
	if (IsDead) return;
	Super::Jump();
}

void AFPSCharacter::CrouchPressed()
{
	Crouch();
}

void AFPSCharacter::CrouchReleased()
{
	UnCrouch();
}

void AFPSCharacter::ActionPressed()
{
	if (PrimaryWeapon == NULL) return;
	ServerRPCStartAction(GetWorld()->GetFirstPlayerController());
}

void AFPSCharacter::ActionReleased()
{
	if (PrimaryWeapon == NULL) return;
	ServerRPCStopAction();
}

void AFPSCharacter::SubactionPressed()
{
	if (PrimaryWeapon == NULL) return;
	ServerRPCStartSubaction();
}

void AFPSCharacter::SubactionReleased()
{
	if (PrimaryWeapon == NULL) return;
	ServerRPCStopSubaction();
}

void AFPSCharacter::ReloadPressed()
{
	if (PrimaryWeapon == NULL) return;
	ServerRPCStartReload();
}

void AFPSCharacter::PickUpWeaponPressed()
{
	if (PrimaryWeapon != NULL) return;
	PickUpWeapon();
}

void AFPSCharacter::DropWeaponPressed()
{
	if (PrimaryWeapon == NULL) return;
	ServerRPCStopAction();
	ServerRPCStopSubaction();
	DropWeapon();
}

void AFPSCharacter::GunShopPressed()
{
	if (HUD->IsOpenGunShop())
	{
		HUD->CloseGunShop();
	}
	else
	{
		HUD->OpenGunShop();
	}
}

bool AFPSCharacter::ServerRPCStartAction_Validate(APlayerController* PlayerController)
{
	if (PlayerController == NULL) return false;
	return true;
}

void AFPSCharacter::ServerRPCStartAction_Implementation(APlayerController* PlayerController)
{
	if (PrimaryWeapon == NULL) return;
	PrimaryWeapon->SetPlayerController(PlayerController);
	PrimaryWeapon->StartAction();
}

void AFPSCharacter::ServerRPCStopAction_Implementation()
{
	if (PrimaryWeapon == NULL) return;
	PrimaryWeapon->StopAction();
}

void AFPSCharacter::ServerRPCStartSubaction_Implementation()
{
	if (PrimaryWeapon == NULL) return;
	PrimaryWeapon->StartSubaction();
}

void AFPSCharacter::ServerRPCStopSubaction_Implementation()
{
	if (PrimaryWeapon == NULL) return;
	PrimaryWeapon->StopSubaction();
}

void AFPSCharacter::ServerRPCStartReload_Implementation()
{
	if (PrimaryWeapon == NULL) return;
	PrimaryWeapon->StartReload();
}

void AFPSCharacter::ServerRPCPickUpWeapon_Implementation()
{
	if (PrimaryWeapon != NULL || PickableWeapon == NULL) return;

	AWeaponBase* WeaponInstance = PickableWeapon->GetWeaponInstance();
	if (WeaponInstance == NULL)
	{
		UBlueprint* WeaponBaseBlueprint = PickableWeapon->GetWeaponBaseBlueprint();
		if (WeaponBaseBlueprint == NULL) return;

		WeaponInstance = AWeaponBase::SpawnWeapon(GetWorld(), WeaponBaseBlueprint->GeneratedClass);
	}
	EquipWeapon(WeaponInstance);

	PickableWeapon->Destroy();
	PickableWeapon = NULL;
}

void AFPSCharacter::ServerRPCDropWeapon_Implementation()
{
	//Get Player direction for adding impulse to PickUpWeapon.
	APlayerController* PlayerController = PrimaryWeapon->GetPlayerController();
	if (PlayerController == NULL) return;
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

bool AFPSCharacter::MulticastRPCSetActorRotation_Validate(FRotator Rotator)
{
	return  true;
}

void AFPSCharacter::MulticastRPCSetActorRotation_Implementation(FRotator Rotator)
{
	SetActorRotation(Rotator);
}

void AFPSCharacter::OnRep_InitializePrimaryWeapon()
{
	if (PrimaryWeapon == NULL) return;
	PrimaryWeapon->Initialize(this);
}

float AFPSCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Log, TEXT("FPSCharacter TakeDamage"));
	if (IsDead) return 0.f;

	float ReducedDamageByArmor = Damage * 0.5f;
	ReducedDamageByArmor = (ReducedDamageByArmor > Armor) ? Armor : ReducedDamageByArmor;
	Armor -= ReducedDamageByArmor;
	Health -= Damage - ReducedDamageByArmor;

	if (Health <= 0)
	{
		Die();
		const float RespawnDelay = 2.f;
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AFPSCharacter::Respawn, RespawnDelay, false, RespawnDelay);
	}

	return Damage;
}

void AFPSCharacter::Die()
{
	UE_LOG(LogTemp, Log, TEXT("Die! : label = %s"), *GetActorLabel());

	IsDead = true;
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Visibility);
	HandsMeshComponent->SetOwnerNoSee(true);

	KnockoutBodyMesh();
	DropWeapon();
}

void AFPSCharacter::Respawn()
{
	UE_LOG(LogTemp, Log, TEXT("Respawn"));

	IsDead = false;
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	HandsMeshComponent->SetOwnerNoSee(false);

	WakeUpBodyMesh();
	InitializeGameplayVariable();
}

void AFPSCharacter::KnockoutBodyMesh()
{
	BodyMeshComponent->SetAllBodiesBelowSimulatePhysics(NamePelvis, true);
	BodyMeshComponent->SetAllBodiesBelowPhysicsBlendWeight(NamePelvis, 1.0f);
}

void AFPSCharacter::WakeUpBodyMesh()
{
	BodyMeshComponent->SetAllBodiesBelowSimulatePhysics(NamePelvis, false);
	BodyMeshComponent->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), NamePelvis);
	BodyMeshComponent->SetRelativeLocation(DefaultLocationOfBodyMeshComponent);
	BodyMeshComponent->SetRelativeRotation(DefaultRotatorOfBodyMeshComponent);
}

void AFPSCharacter::EquipWeapon(AWeaponBase* WeaponBase)
{
	// Attach AWeaponBase Actor for first-person view
	PrimaryWeapon = WeaponBase;
	FName AttachingGripPointName = PrimaryWeapon->GetAttachingGripPointName();
	PrimaryWeapon->AttachToComponent(HandsMeshComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), AttachingGripPointName);
	PrimaryWeapon->Initialize(this);

	// Attach AWeaponModelForBody for third-person view
	WeaponModelForBody = PrimaryWeapon->SpawnWeaponModelForBodyActor();
	if (WeaponModelForBody == NULL) return;
	WeaponModelForBody->SetOwner(this);
	WeaponModelForBody->AttachToComponent(BodyMeshComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), AttachingGripPointName);
}

AWeaponBase* AFPSCharacter::UnEquipWeapon()
{
	if (PrimaryWeapon == NULL) return NULL;

	// UnEquip PrimaryWeapon
	AWeaponBase* WeaponInstance = PrimaryWeapon;
	PrimaryWeapon = NULL;
	WeaponInstance->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	WeaponInstance->SetOwner(NULL);
	WeaponInstance->OnUnEquipped();

	// UnEquip WeaponModelForThirdPerson
	if (WeaponModelForBody == NULL) return WeaponInstance;
	WeaponModelForBody->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	WeaponModelForBody->Destroy();

	return WeaponInstance;
}

void AFPSCharacter::DropWeapon()
{
	if (PrimaryWeapon == NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("DropWeapon() : PrimaryWeapon == NULL"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("DropWeapon() : dropped!"));
	ServerRPCDropWeapon();
}

void AFPSCharacter::PickUpWeapon()
{
	UE_LOG(LogTemp, Log, TEXT("PickUpWeapon()"));
	ServerRPCPickUpWeapon();
}

float AFPSCharacter::GetHealth()
{
	return Health;
}

float AFPSCharacter::GetArmor()
{
	return Armor;
}

UCameraComponent* AFPSCharacter::GetCameraComponent()
{
	return CameraComponent;
}

AWeaponBase* AFPSCharacter::GetPrimaryWeapon()
{
	return PrimaryWeapon;
}

USkeletalMeshComponent* AFPSCharacter::GetHandsMeshComponent()
{
	return HandsMeshComponent;
}

USkeletalMeshComponent* AFPSCharacter::GetBodyMeshComponent()
{
	return BodyMeshComponent;
}

float AFPSCharacter::GetAimPtich()
{
	return AimPitch;
}

float AFPSCharacter::GetAimYaw()
{
	return AimYaw;
}

void AFPSCharacter::SetPickableWeapon(APickUpWeapon* Instance)
{
	PickableWeapon = Instance;
}