// Fill out your copyright notice in the Description page of Project Settings.

#include "TimeTravelController.h"

// Sets default values
ATimeTravelController::ATimeTravelController()
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
void ATimeTravelController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATimeTravelController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void ATimeTravelController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Binding of axis mappings
	PlayerInputComponent->BindAxis("MoveForward", this, &ATimeTravelController::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATimeTravelController::MoveRight);
	PlayerInputComponent->BindAxis("CameraHorizontal", this, &ATimeTravelController::MoveCameraHorizontal);
	PlayerInputComponent->BindAxis("CameraVertical", this, &ATimeTravelController::MoveCameraVertical);

	// Binding of action mappings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATimeTravelController::Jump);
	PlayerInputComponent->BindAction("Ability", IE_Repeat, this, &ATimeTravelController::Recall);
}

void ATimeTravelController::Jump()
{
	Super::Jump();
}

void ATimeTravelController::MoveForward(float mValue)
{
	AddMovementInput(GetActorForwardVector(), mValue);
}

void ATimeTravelController::MoveRight(float mValue)
{
	AddMovementInput(GetActorRightVector(), mValue);
}

void ATimeTravelController::MoveCameraHorizontal(float mAmount)
{
	AddControllerYawInput(mAmount);
}

void ATimeTravelController::MoveCameraVertical(float mAmount)
{
	AddControllerPitchInput(mAmount);
}

void ATimeTravelController::Recall()
{
	UE_LOG(LogTemp, Error, TEXT("Recall() activated."));
}

