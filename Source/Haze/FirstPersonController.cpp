// Fill out your copyright notice in the Description page of Project Settings.

#include "FirstPersonController.h"

// Sets default values
AFirstPersonController::AFirstPersonController()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	SpringArm->SetWorldLocation(FVector(0.0f, 0.0f, 48.0f));
	SpringArm->TargetArmLength = 0.0f;
	SpringArm->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AFirstPersonController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFirstPersonController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFirstPersonController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Binding of axis mappings
	PlayerInputComponent->BindAxis("MoveForward", this, &AFirstPersonController::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFirstPersonController::MoveRight);
	PlayerInputComponent->BindAxis("CameraHorizontal", this, &AFirstPersonController::MoveCameraHorizontal);
	PlayerInputComponent->BindAxis("CameraVertical", this, &AFirstPersonController::MoveCameraVertical);

	// Binding of action mappings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFirstPersonController::Jump);
}

void AFirstPersonController::Jump()
{
	Super::Jump();
}

void AFirstPersonController::MoveForward(float mValue)
{
	AddMovementInput(GetActorForwardVector(), mValue);
	
}

void AFirstPersonController::MoveRight(float mValue)
{
	AddMovementInput(GetActorRightVector(), mValue);
}

void AFirstPersonController::MoveCameraHorizontal(float mAmount)
{
	AddControllerYawInput(mAmount);
}

void AFirstPersonController::MoveCameraVertical(float mAmount)
{
	AddControllerPitchInput(mAmount);
}

