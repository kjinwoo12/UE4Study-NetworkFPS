// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS.h"
#include "FPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Camera
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCameraComponent"));
	FPSCameraComponent->SetupAttachment(GetCapsuleComponent());
	FPSCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, BaseEyeHeight));
	FPSCameraComponent->bUsePawnControlRotation = true;

	//Mesh for camera only
	USkeletalMeshComponent* DefaultMesh = GetMesh();
	DefaultMesh->SetOnlyOwnerSee(true);
	DefaultMesh->SetupAttachment(FPSCameraComponent);
	DefaultMesh->bCastDynamicShadow = false;
	DefaultMesh->CastShadow = false;

}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("We are using FPSCharacter."));
	}
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

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::Jump);
}

void AFPSCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void AFPSCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}