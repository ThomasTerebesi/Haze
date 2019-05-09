// Fill out your copyright notice in the Description page of Project Settings.

#include "TimeTravelController.h"

// Sets default values
ATimeTravelController::ATimeTravelController()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereCollider");

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);
	SphereComponent->SetupAttachment(RootComponent);

	SpringArm->SetWorldLocation(FVector(0.0f, 0.0f, 48.0f));
	SpringArm->TargetArmLength = 0.0f;
	SpringArm->bUsePawnControlRotation = true;

	SphereComponent->SetSphereRadius(128.0f);	// TODO: Make this sphere component collide so that player cannot recall outside of level limits

	EnableDebug = true;
	ShowRecallTransforms = true;

	CanSetPosition = true;
	StorePositionDelay = 0.2f;
	MaxStoredRecallTransforms = 64;
	CanRecall = true;
	RecallPressed = false;
	RecallStopped = false;
	RecallLocationSpeed = 10.0f;
	RecallRotationSpeed = 7.0f;
	RecallTransformCounter = 0;
	RecallCooldown = 6.0f;
	RecallTolerance = 128.0f;
	
	IsRunningForward = false;
	StandardFieldOfView = 100.0f;
	ForwardFieldOfView = 104.0f;
	FieldOfViewSpeed = 6.0f;

	Camera->FieldOfView = StandardFieldOfView;
}

// Called when the game starts or when spawned
void ATimeTravelController::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(StorePositionDelayHandle, this, &ATimeTravelController::AddRecallTransform, StorePositionDelay, true);
}

// Called every frame
void ATimeTravelController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanSetPosition)
	{
		if (RecallTransforms.Num() >= MaxStoredRecallTransforms)
		{
			RecallTransforms.RemoveAt(0);

			if (EnableDebug && ShowRecallTransforms)
			{
				UE_LOG(LogTemp, Warning, TEXT("Removed: %s"), *RecallTransforms[0].ToString());
			}
		}
	}

	if (RecallTransforms.Num() > 0 && RecallPressed)
	{
		SetActorLocation(FMath::VInterpTo(GetActorTransform().GetLocation(), RecallTransforms[RecallTransformCounter].GetLocation(), DeltaTime, RecallLocationSpeed));
		SetActorRotation(FMath::RInterpTo(GetActorTransform().GetRotation().Rotator(), RecallTransforms[RecallTransformCounter].GetRotation().Rotator(), DeltaTime, RecallRotationSpeed));

		if (UKismetMathLibrary::NearlyEqual_TransformTransform(GetActorTransform(), RecallTransforms[RecallTransformCounter], RecallTolerance, RecallTolerance, RecallTolerance))
		{
			// Stop recall and remove only the used elements from RecallTransformsArray
			if (RecallStopped)
			{
				CanSetPosition = true;
				RecallPressed = false;
				RecallStopped = false;
				GetWorld()->GetTimerManager().SetTimer(RecallCooldownHandle, this, &ATimeTravelController::ResetRecallCooldown, RecallCooldown, false);

				for (int8 i = RecallTransforms.Num() - 1; i >= RecallTransformCounter; i--)
				{
					if (EnableDebug && ShowRecallTransforms)
					{
						UE_LOG(LogTemp, Warning, TEXT("Removing RecallTransforms[%d]."), i);
					}

					RecallTransforms.RemoveAt(i);
				}
			}

			if (RecallTransformCounter != 0)
			{
				RecallTransformCounter--;
			}
			else
			{
				CanSetPosition = true;
				RecallPressed = false;
				RecallTransforms.Empty();
				GetWorld()->GetTimerManager().SetTimer(RecallCooldownHandle, this, &ATimeTravelController::ResetRecallCooldown, RecallCooldown, false);
			}
		}
	}

	// Setting the camera component's field of view when running forward
	if (IsRunningForward && Camera->FieldOfView < ForwardFieldOfView)
	{
		Camera->FieldOfView = FMath::FInterpTo(Camera->FieldOfView, ForwardFieldOfView, DeltaTime, FieldOfViewSpeed);
	}
	else
	{
		if (Camera->FieldOfView > StandardFieldOfView)
		{
			Camera->FieldOfView = FMath::FInterpTo(Camera->FieldOfView, StandardFieldOfView, DeltaTime, FieldOfViewSpeed);
		}
	}
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
	PlayerInputComponent->BindAction("Ability", IE_Pressed, this, &ATimeTravelController::Recall);
}

void ATimeTravelController::Jump()
{
	Super::Jump();
}

void ATimeTravelController::MoveForward(float mValue)
{
	// Determining whether player character is running forward in order to set the camera component's field of view accordingly
	mValue > 0 ? IsRunningForward = true : IsRunningForward = false;

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
	if (CanRecall)
	{
		RecallPressed = true;
		CanRecall = false;
		CanSetPosition = false;

		RecallTransformCounter = RecallTransforms.Num() - 1;
	} 
	else if (RecallPressed)
	{
		RecallStopped = true;
	}
}

void ATimeTravelController::ResetRecallCooldown()
{
	CanRecall = true;
}

void ATimeTravelController::AddRecallTransform()
{
	RecallTransforms.Add(GetActorTransform());
	if (EnableDebug && ShowRecallTransforms)
	{
		UE_LOG(LogTemp, Warning, TEXT("Added: %s"), *GetActorTransform().ToString());
	}
}