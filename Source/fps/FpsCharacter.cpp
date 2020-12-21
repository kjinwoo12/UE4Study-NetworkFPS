// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS.h"
#include "FPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "WeaponBase.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
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

	// PrimaryWeapon will be loaded at BeginPlay();
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("We are using FPSCharacter."));
	}

	EquipTestGun();
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFPSCharacter::Reload);
}

void AFPSCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void AFPSCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void AFPSCharacter::StartAction()
{
	if (PrimaryWeapon == NULL) return;
	PrimaryWeapon->StartAction();
}

void AFPSCharacter::StopAction()
{
	if (PrimaryWeapon == NULL) return;
	PrimaryWeapon->StopAction();
}

void AFPSCharacter::StartSubaction()
{
	if (PrimaryWeapon == NULL) return;
	PrimaryWeapon->StartSubaction();
}

void AFPSCharacter::StopSubaction()
{
	if (PrimaryWeapon == NULL) return;
	PrimaryWeapon->StopSubaction();
}

void AFPSCharacter::Reload()
{
	if (PrimaryWeapon == NULL) return;
	PrimaryWeapon->Reload();
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