// Fill out your copyright notice in the Description page of Project Settings.

#include "TimeTravelController.h"

// Sets default values
ATimeTravelController::ATimeTravelController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// "Debug" category setup
	EnableDebug = true;
	ShowRecallTransforms = true;

	// "Recall" category setup
	CanSetPosition = true;
	StorePositionDelay = 0.05f;
	MaxStoredRecallTransforms = 256;
	CanRecall = true;
	IsRecallActive = false;
	RecallStopped = false;
	RecallLocationSpeed = 32.0f;
	RecallRotationSpeed = 8.0f;
	RecallTransformCounter = 0;
	RecallCooldown = 4.0f;
	RecallTolerance = 128.0f;
	EnableRecallRotation = false;

	// "Other" category setup
	IsRunningForward = false;
	StandardFieldOfView = 100.0f;
	ForwardFieldOfView = 104.0f;
	RecallFieldOfView = 88.0f;
	FieldOfViewSpeed = 8.0f;

	// Component setup
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	SpringArm->SetWorldLocation(FVector(0.0f, 0.0f, 48.0f));
	SpringArm->TargetArmLength = 0.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera->FieldOfView = StandardFieldOfView;

	GetCapsuleComponent()->SetCapsuleRadius(80.0f);
}

// Called when the game starts or when spawned
void ATimeTravelController::BeginPlay()
{
	Super::BeginPlay();

	// Adding new elements to RecallTransforms in regular intervals
	GetWorld()->GetTimerManager().SetTimer(StorePositionDelayHandle, this, &ATimeTravelController::AddRecallTransformToArray, StorePositionDelay, true);
}

// Called every frame
void ATimeTravelController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RemoveRecallTransforms();
	Recall(DeltaTime);
	SetFieldOfView(DeltaTime);
}

// Removes elements from RecallTransforms if the number of elements is above a certain limit
void ATimeTravelController::RemoveRecallTransforms()
{
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
}

// Sets the camera component's field of view depending on the situation
void ATimeTravelController::SetFieldOfView(const float & mDeltaTime)
{

	if (IsRunningForward && !IsRecallActive && Camera->FieldOfView < ForwardFieldOfView)
	{
		Camera->FieldOfView = FMath::FInterpTo(Camera->FieldOfView, ForwardFieldOfView, mDeltaTime, FieldOfViewSpeed);
	}
	else if (IsRecallActive && Camera->FieldOfView > RecallFieldOfView)
	{
		Camera->FieldOfView = FMath::FInterpTo(Camera->FieldOfView, RecallFieldOfView, mDeltaTime, FieldOfViewSpeed);
	}
	else if (Camera->FieldOfView != StandardFieldOfView)
	{
		Camera->FieldOfView = FMath::FInterpTo(Camera->FieldOfView, StandardFieldOfView, mDeltaTime, FieldOfViewSpeed);
	}
}

// Handles the recall ability
void ATimeTravelController::Recall(const float & mDeltaTime)
{
	if (RecallTransforms.Num() > 0 && IsRecallActive)
	{
		SetActorLocation(UKismetMathLibrary::TLerp(GetActorTransform(), RecallTransforms[RecallTransformCounter], RecallLocationSpeed * mDeltaTime).GetLocation());

		// Rotation is disabled by default
		if (EnableRecallRotation)
		{
			GetController()->SetControlRotation(UKismetMathLibrary::RLerp(GetActorRotation(), RecallTransforms[RecallTransformCounter].GetRotation().Rotator(), RecallRotationSpeed * mDeltaTime, false));
		}

		if (UKismetMathLibrary::NearlyEqual_TransformTransform(GetActorTransform(), RecallTransforms[RecallTransformCounter], RecallTolerance, RecallTolerance, RecallTolerance))
		{
			// Stop recall and remove only the used elements from RecallTransformsArray
			if (RecallStopped)
			{
				for (int8 i = RecallTransforms.Num() - 1; i >= RecallTransformCounter; i--)
				{
					if (EnableDebug && ShowRecallTransforms)
					{
						UE_LOG(LogTemp, Error, TEXT("Removed: %s."), *RecallTransforms[i].ToString());
					}

					RecallTransforms.RemoveAt(i);
				}

				CanSetPosition = true;
				IsRecallActive = false;
				RecallStopped = false;
				GetWorld()->GetTimerManager().SetTimer(RecallCooldownHandle, this, &ATimeTravelController::ResetRecallCooldown, RecallCooldown, false);
			}

			if (RecallTransformCounter != 0)
			{
				RecallTransformCounter--;
			}
			else
			{
				CanSetPosition = true;
				IsRecallActive = false;
				RecallTransforms.Empty();
				GetWorld()->GetTimerManager().SetTimer(RecallCooldownHandle, this, &ATimeTravelController::ResetRecallCooldown, RecallCooldown, false);

				if (EnableDebug && ShowRecallTransforms)
				{
					UE_LOG(LogTemp, Error, TEXT("Cleared RemoveTransforms."));
				}
			}
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
	mValue > 0.0f ? IsRunningForward = true : IsRunningForward = false;

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
	if (RecallTransforms.Num() > 0)
	{
		// Recall if the cooldown is not active, else abort an active recall
		if (CanRecall)
		{
			IsRecallActive = true;
			CanRecall = false;
			CanSetPosition = false;

			RecallTransformCounter = RecallTransforms.Num() - 1;
		}
		else if (IsRecallActive)
		{
			RecallStopped = true;
		}
	}
	else
	{
		if (EnableDebug && ShowRecallTransforms)
		{
			UE_LOG(LogTemp, Error, TEXT("RecallTransforms is empty."));
		}
	}
}

void ATimeTravelController::ResetRecallCooldown()
{
	CanRecall = true;
}

void ATimeTravelController::AddRecallTransformToArray()
{
	// Only do this while recall is inactive
	if (CanSetPosition)
	{
		RecallTransforms.Add(GetActorTransform());

		if (EnableDebug && ShowRecallTransforms)
		{
			UE_LOG(LogTemp, Warning, TEXT("Added: %s"), *GetActorTransform().ToString());
		}
	}
}
