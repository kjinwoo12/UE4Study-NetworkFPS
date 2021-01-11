// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSCharacter.h"
#include "FPS.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "WeaponBase.h"
#include "FPSHUD.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

#include "DrawDebugHelpers.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
	SetReplicates(true);

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// It's named `CharacterCapsuleComponent` for successful compiling because the name `CapsuleComponent` is already exist in ACharacter
	UCapsuleComponent* CharacterCapsuleComponent = GetCapsuleComponent();
	CharacterCapsuleComponent->SetCapsuleHalfHeight(96.0f);
	CharacterCapsuleComponent->SetCapsuleRadius(42.0f);

	// Camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCameraComponent"));
	CameraComponent->SetupAttachment(CharacterCapsuleComponent);
	CameraComponent->SetRelativeLocation(FVector(0.f, 0.f, BaseEyeHeight));
	CameraComponent->bUsePawnControlRotation = true;

	// Hand mesh for camera only
	HandsMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandsMesh"));
	HandsMeshComponent->SetOnlyOwnerSee(true);
	HandsMeshComponent->SetupAttachment(CameraComponent);
	HandsMeshComponent->bCastDynamicShadow = false;
	HandsMeshComponent->CastShadow = false;
	HandsMeshComponent->SetRelativeLocation(FVector(-25, 15, -150.f));
	HandsMeshComponent->SetRelativeRotation(FRotator(-7.f, -15.f, 0.f));

	// BodyMesh for others
	USkeletalMeshComponent* BodyMesh = GetMesh();
	BodyMesh->SetOwnerNoSee(true);
	BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, -97.f));
	BodyMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	MovementComponent = ACharacter::GetCharacterMovement();
	MovementComponent->bUseFlatBaseForFloorChecks = true;

	// Gameplay variable
	MaxHealth = 100;
	Health = 100;
	MaxArmor = 100;
	Armor = 50;
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

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickCrosshair();
}

void AFPSCharacter::TickCrosshair()
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

	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::Jump);

	// Actions
	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AFPSCharacter::StartAction);
	PlayerInputComponent->BindAction("Action", IE_Released, this, &AFPSCharacter::StopAction);
	PlayerInputComponent->BindAction("Subaction", IE_Pressed, this, &AFPSCharacter::StartSubaction);
	PlayerInputComponent->BindAction("Subaction", IE_Released, this, &AFPSCharacter::StopSubaction);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFPSCharacter::StartReload);
}

void AFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSCharacter, Health);
	DOREPLIFETIME(AFPSCharacter, Armor);
}

void AFPSCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void AFPSCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

float AFPSCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageAbsolption = Damage * 0.5f;
	DamageAbsolption = (DamageAbsolption > Armor) ? Armor : DamageAbsolption;
	Armor -= DamageAbsolption;
	Health -= Damage - DamageAbsolption;
	return Damage;
}

void AFPSCharacter::StartAction_Implementation()
{
	if (PrimaryWeapon == NULL) return;

	MulticastRPCStartAction();
}

void AFPSCharacter::StopAction_Implementation()
{
	if (PrimaryWeapon == NULL) return;

	MulticastRPCStopAction();
}

void AFPSCharacter::StartSubaction_Implementation()
{
	if (PrimaryWeapon == NULL) return;

	MulticastRPCStartSubaction();
}

void AFPSCharacter::StopSubaction_Implementation()
{
	if (PrimaryWeapon == NULL) return;

	MulticastRPCStopSubaction();
}

void AFPSCharacter::StartReload_Implementation()
{
	if (PrimaryWeapon == NULL) return;

	MulticastRPCStartReload();
}


void AFPSCharacter::MulticastRPCStartAction_Implementation()
{
	PrimaryWeapon->StartAction();
}

void AFPSCharacter::MulticastRPCStopAction_Implementation()
{
	PrimaryWeapon->StopAction();
}

void AFPSCharacter::MulticastRPCStartSubaction_Implementation()
{
	PrimaryWeapon->StartSubaction();
}

void AFPSCharacter::MulticastRPCStopSubaction_Implementation()
{
	PrimaryWeapon->StopSubaction();
}

void AFPSCharacter::MulticastRPCStartReload_Implementation()
{
	PrimaryWeapon->StartReload();
}

void AFPSCharacter::EquipWeapon(FString WeaponReference)
{
	UClass* GeneratedBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *WeaponReference));
	PrimaryWeapon = GetWorld()->SpawnActor<AWeaponBase>(GeneratedBP, FVector(0, 0, 0), FRotator::ZeroRotator);
	PrimaryWeapon->AttachToComponent(HandsMeshComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	PrimaryWeapon->SetParentAnimInstance(HandsMeshComponent->GetAnimInstance());
	PrimaryWeapon->GetLineTraceCollisionQueryParams()->AddIgnoredActor(this);
}

void AFPSCharacter::UnEquipWeapon()
{
	PrimaryWeapon->GetLineTraceCollisionQueryParams()->ClearIgnoredActors();
	PrimaryWeapon->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	PrimaryWeapon->Destroy();
	PrimaryWeapon = NULL;
}

void AFPSCharacter::EquipTestGun()
{
	EquipWeapon("Class'/Game/MyContent/Blueprints/BP_WeaponBase_TestGun.BP_WeaponBase_TestGun_C'");
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

bool AFPSCharacter::LineTrace(FHitResult& HitResult)
{
	// Get Player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);

	// Get end point
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * 5000;

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
	// CollisionParams for LineTrace
	FCollisionQueryParams LineTraceCollisionQueryParams;

	bool IsHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		PlayerViewPointLocation,
		LineTraceEnd,
		ECollisionChannel::ECC_Pawn,
		LineTraceCollisionQueryParams
	);

	return IsHit;
}