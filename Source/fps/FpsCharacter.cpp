// Fill out your copyright notice in the Description page of Project Settings.

#include "fps.h"
#include "FpsCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AFpsCharacter::AFpsCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Camera
	FpsCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FpsCameraComponent"));
	FpsCameraComponent->SetupAttachment(GetCapsuleComponent());
	FpsCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, BaseEyeHeight));
	FpsCameraComponent->bUsePawnControlRotation = true;

	//Mesh for camera only
	USkeletalMeshComponent* DefaultMesh = GetMesh();
	DefaultMesh->SetOnlyOwnerSee(true);
	DefaultMesh->SetupAttachment(FpsCameraComponent);
	DefaultMesh->bCastDynamicShadow = false;
	DefaultMesh->CastShadow = false;

}

// Called when the game starts or when spawned
void AFpsCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("We are using FpsCharacter."));
	}
}

// Called every frame
void AFpsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFpsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFpsCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFpsCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AFpsCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AFpsCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFpsCharacter::Jump);
}

void AFpsCharacter::MoveForward(float Value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AFpsCharacter::MoveRight(float Value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}