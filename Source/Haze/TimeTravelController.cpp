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
	StorePositionDelay = 0.1f;
	MaxStoredRecallTransforms = 128;
	CanRecall = true;
	RecallPressed = false;
	RecallStopped = false;
	RecallLocationSpeed = 10.0f;
	RecallRotationSpeed = 7.0f;
	RecallTransformCounter = 0;
	RecallCooldown = 6.0f;
	RecallTolerance = 128.0f;
	
	// "Other" category setup
	IsRunningForward = false;
	StandardFieldOfView = 100.0f;
	ForwardFieldOfView = 104.0f;
	RecallFieldOfView = 80.0f;
	FieldOfViewSpeed = 6.0f;
	
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
	GetWorld()->GetTimerManager().SetTimer(StorePositionDelayHandle, this, &ATimeTravelController::AddRecallTransform, StorePositionDelay, true);
}

// Called every frame
void ATimeTravelController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanSetPosition)
	{
		// Removing elements from RecallTransforms if the number of elements is above a certain threshold
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
		// TODO: Player character gets stuck when a position is in the air, needs to jump to continue; re-research and maybe use a function from Kismet/Blueprint?
		SetActorLocation(FMath::VInterpTo(GetActorTransform().GetLocation(), RecallTransforms[RecallTransformCounter].GetLocation(), DeltaTime, RecallLocationSpeed));
		SetActorRotation(FMath::RInterpTo(GetActorTransform().GetRotation().Rotator(), RecallTransforms[RecallTransformCounter].GetRotation().Rotator(), DeltaTime, RecallRotationSpeed));

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
				RecallPressed = false;
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
				RecallPressed = false;
				RecallTransforms.Empty();
				GetWorld()->GetTimerManager().SetTimer(RecallCooldownHandle, this, &ATimeTravelController::ResetRecallCooldown, RecallCooldown, false);

				if (EnableDebug && ShowRecallTransforms)
				{
					UE_LOG(LogTemp, Error, TEXT("Cleared RemoveTransforms."));
				}
			}
		}
	}

	// Setting the camera component's field of view depending on the situation
	if (IsRunningForward && Camera->FieldOfView < ForwardFieldOfView)
	{
		Camera->FieldOfView = FMath::FInterpTo(Camera->FieldOfView, ForwardFieldOfView, DeltaTime, FieldOfViewSpeed);
	}
	else if (RecallPressed && Camera->FieldOfView > RecallFieldOfView)
	{
		Camera->FieldOfView = FMath::FInterpTo(Camera->FieldOfView, RecallFieldOfView, DeltaTime, FieldOfViewSpeed);
	}
	else if (Camera->FieldOfView != StandardFieldOfView)
	{
		Camera->FieldOfView = FMath::FInterpTo(Camera->FieldOfView, StandardFieldOfView, DeltaTime, FieldOfViewSpeed);
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
	if (RecallTransforms.Num() > 0)
	{	
		// Recall if the cooldown is not active, else abort an active recall
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

void ATimeTravelController::AddRecallTransform()
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