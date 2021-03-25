// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSCharacter.h"
#include "FPS.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "WeaponBase.h"
#include "PickUpWeapon.h"
#include "FPSHUD.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

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
	MovementComponent->MaxWalkSpeed = 200;
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
	bIsDead = false;
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

	EquipTestGun();
}

void AFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSCharacter, Health);
	DOREPLIFETIME(AFPSCharacter, Armor);
	DOREPLIFETIME(AFPSCharacter, PickableWeapon);
	DOREPLIFETIME(AFPSCharacter, PrimaryWeapon);
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ClientRPCTickCrosshair();
}

void AFPSCharacter::ClientRPCTickCrosshair_Implementation()
{
	if (PrimaryWeapon == NULL || HUD == NULL) return;
	const float CharacterSpeedOffset = GetVelocity().Size() / PrimaryWeapon->GetMovementStability();
	const int JumpingOffset = (MovementComponent->IsFalling()) ? 30 : 0;
	const float CrosshairCenterOffset = CharacterSpeedOffset + JumpingOffset;
	HUD->SetCrosshairCenterOffset(CrosshairCenterOffset);
}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

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
	PlayerInputComponent->BindAction("PickUpWeapon", IE_Pressed, this, &AFPSCharacter::PickUpWeapon);
	PlayerInputComponent->BindAction("DropWeapon", IE_Pressed, this, &AFPSCharacter::DropWeapon);
}

void AFPSCharacter::MoveForward(float Value)
{
	if (bIsDead) return;
	AddMovementInput(GetActorForwardVector(), Value);
}

void AFPSCharacter::MoveRight(float Value)
{
	if (bIsDead) return;
	AddMovementInput(GetActorRightVector(), Value);
}

void AFPSCharacter::AddControllerPitchInput(float Value)
{
	if (bIsDead) return;
	Super::AddControllerPitchInput(Value);
}

void AFPSCharacter::AddControllerYawInput(float Value)
{
	if (bIsDead) return;
	Super::AddControllerYawInput(Value);
}

void AFPSCharacter::Jump()
{
	if (bIsDead) return;
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
	ServerRPCStartAction(GetWorld()->GetFirstPlayerController());
}

void AFPSCharacter::ActionReleased()
{
	ServerRPCStopAction();
}

void AFPSCharacter::SubactionPressed()
{
	ServerRPCStartSubaction();
}

void AFPSCharacter::SubactionReleased()
{	
	ServerRPCStopSubaction();
}

void AFPSCharacter::ReloadPressed()
{
	ServerRPCStartReload();
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

bool AFPSCharacter::ServerRPCRequestEquipWeaponMulticast_Validate(AWeaponBase* WeaponBase)
{
	return true;
}

void AFPSCharacter::ServerRPCRequestEquipWeaponMulticast_Implementation(AWeaponBase* WeaponBase)
{
	MulticastRPCEquipWeapon(WeaponBase);
}

bool AFPSCharacter::MulticastRPCEquipWeapon_Validate(AWeaponBase* WeaponBase)
{
	return true;
}

void AFPSCharacter::MulticastRPCEquipWeapon_Implementation(AWeaponBase* WeaponBase)
{
	PrimaryWeapon = WeaponBase;
	if (GetNetMode() == ENetMode::NM_ListenServer)
	{
		UE_LOG(LogTemp, Log, TEXT("MulticastRPCEquipWeapon() : Server"));
		PrimaryWeapon->AttachToComponent(HandsMeshComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), NameGripPoint);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("MulticastRPCEquipWeapon() : Client"));
	}

	PrimaryWeapon->SetOwner(this);
	PrimaryWeapon->SetParentAnimInstance(HandsMeshComponent->GetAnimInstance());

	// For Ignore owner's character when line-trace.
	PrimaryWeapon->GetLineTraceCollisionQueryParams()->AddIgnoredActor(this);
}

void AFPSCharacter::ServerRPCPickUpWeapon_Implementation()
{
	if (PickableWeapon == NULL) return;

	AWeaponBase* WeaponInstance = PickableWeapon->GetWeaponInstance();
	if (WeaponInstance == NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("WeaponInstance == NULL"));
		return;
	}
	EquipWeapon(WeaponInstance);

	PickableWeapon->Destroy();
	PickableWeapon = NULL;
}

void AFPSCharacter::ServerRPCDropWeapon_Implementation()
{
	AWeaponBase* WeaponInstance = UnEquipWeapon();
	APickUpWeapon* PickUpWeapon = WeaponInstance->SpawnPickUpWeaponActor();
	PickUpWeapon->SetWeaponInstance(WeaponInstance);
}

float AFPSCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead) return 0.f;

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

	bIsDead = true;
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Visibility);
	HandsMeshComponent->SetOwnerNoSee(true);

	KnockoutBodyMesh();
	DropWeapon();
}

void AFPSCharacter::Respawn()
{
	UE_LOG(LogTemp, Log, TEXT("Respawn"));

	bIsDead = false;
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
	ServerRPCRequestEquipWeaponMulticast(WeaponBase);
}

AWeaponBase* AFPSCharacter::UnEquipWeapon()
{
	if (PrimaryWeapon == NULL) return NULL;
	AWeaponBase* WeaponInstance = PrimaryWeapon;
	PrimaryWeapon = NULL;
	WeaponInstance->SetOwner(NULL);
	WeaponInstance->GetLineTraceCollisionQueryParams()->ClearIgnoredActors();
	WeaponInstance->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
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

void AFPSCharacter::EquipTestGun()
{
	EquipWeapon(AWeaponBase::SpawnWeapon(GetWorld(), "Class'/Game/MyContent/Weapon/BP_WeaponBase_TestGun.BP_WeaponBase_TestGun_C'"));
}

float AFPSCharacter::GetHealth()
{
	return Health;
}

float AFPSCharacter::GetArmor()
{
	return Armor;
}

AWeaponBase* AFPSCharacter::GetPrimaryWeapon()
{
	return PrimaryWeapon;
}

void AFPSCharacter::SetPickableWeapon(APickUpWeapon* Instance)
{
	PickableWeapon = Instance;
}