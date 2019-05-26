// Fill out your copyright notice in the Description page of Project Settings.

#include "TimeTravelController.h"


// Sets default values
ATimeTravelController::ATimeTravelController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// "Debug" category initialisation
	EnableDebug = true;
	EnableRecallDebug = true;
	EnableWallClimbDebug = true;

	// "Recall" category initialisation
	CanSetPosition = true;
	StorePositionDelay = 0.04f;
	MaxStoredRecallTransforms = 1024;
	CanRecall = true;
	IsRecallActive = false;
	RecallStopped = false;
	RecallLocationSpeed = 24.0f;
	RecallRotationSpeed = 8.0f;
	RecallTransformCounter = 0;
	RecallCooldown = 4.0f;
	RecallTolerance = 192.0f;
	EnableRecallRotation = false;

	// "Wall Climb" category initialisation
	EnableWallClimb = true;
	ClimbTime = 0.0f;
	ClimbTimeMax = 1.2f;
	ClimbSpeed = 420.0f;
	IsWallClimbing = false;

	// "Wall Run" category initialisation
	WallRunSpeed = 20480.0f;
	WallRunDirection = FVector(0.0f);
	IsWallRunning = false;

	// "Object Pick Up" category initialisation
	IsHoldingObject = false;

	// "General" category initialisation
	MaxJumps = 2;
	JumpCount = 0;
	JumpLaunchVelocity = 420.0f;
	IsRunningForward = false;
	StandardFieldOfView = 100.0f;
	ForwardFieldOfView = 104.0f;
	RecallFieldOfView = 88.0f;
	FieldOfViewSpeed = 8.0f;

	// Component creation
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	WallClimbLineTraceEnd = CreateDefaultSubobject<USceneComponent>("WallClimbLineTraceEnd");

	WallRunCollision = CreateDefaultSubobject<UBoxComponent>("WallRunCollision");

	ObjectPickUpLineTraceEnd = CreateDefaultSubobject<USceneComponent>("ObjectPickUpLineTraceEnd");
	ObjectPickUpPhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>("ObjectPickUpPhysicsHandle");

	PostProcess = CreateDefaultSubobject<UPostProcessComponent>("PostProcess");

	// Component hierarchy
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	WallClimbLineTraceEnd->SetupAttachment(RootComponent);

	WallRunCollision->SetupAttachment(RootComponent);

	ObjectPickUpLineTraceEnd->SetupAttachment(Camera);


	// Component setup
	SpringArm->SetWorldLocation(FVector(0.0f, 0.0f, 48.0f));
	SpringArm->TargetArmLength = 0.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera->FieldOfView = StandardFieldOfView;

	WallClimbLineTraceEnd->SetWorldLocation(FVector(88.0f, 0.0f, 0.0f));

	WallRunCollision->SetBoxExtent(FVector(56.0f, 92.0f, 56.0f));

	ObjectPickUpLineTraceEnd->SetWorldLocation(FVector(320.0f, 0.0f, 0.0f));

	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(80.0f);

	GetCharacterMovement()->AirControl = 0.5;

	// Delegate setup
	WallRunCollision->OnComponentBeginOverlap.AddDynamic(this, &ATimeTravelController::OnWallRunCollisionBegin);
	WallRunCollision->OnComponentEndOverlap.AddDynamic(this, &ATimeTravelController::OnWallRunCollisionEnd);

	LandedDelegate.AddDynamic(this, &ATimeTravelController::OnCharacterLanded);
}

// Called when the game starts or when spawned
void ATimeTravelController::BeginPlay()
{
	Super::BeginPlay();

	// Adding new elements to RecallTransforms in regular intervals
	GetWorld()->GetTimerManager().SetTimer(StorePositionDelayHandle, this, &ATimeTravelController::AddRecallTransformToArray, StorePositionDelay, true);

	GetCharacterMovement()->SetPlaneConstraintEnabled(true);
}

// Called every frame
void ATimeTravelController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RecallUpdate(DeltaTime);
	WallClimbUpdate();
	WallRunUpdate();
	ObjectPickUpUpdate();
	SetFieldOfView(DeltaTime);
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
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ATimeTravelController::StopJump);
	PlayerInputComponent->BindAction("Ability", IE_Pressed, this, &ATimeTravelController::Recall);
	PlayerInputComponent->BindAction("ObjectPickUp", IE_Pressed, this, &ATimeTravelController::HandleObjectPickUpInput);
}

void ATimeTravelController::Jump()
{
	if (JumpCount < MaxJumps)
	{
		LaunchCharacter(FVector(0.0f, 0.0f, JumpLaunchVelocity), false, true);
		JumpCount++;
	}
}

void ATimeTravelController::StopJump()
{
	Super::StopJumping();
}

void ATimeTravelController::OnCharacterLanded(const FHitResult & mHit)
{
	JumpCount = 0;
}

void ATimeTravelController::MoveForward(float mValue)
{
	if (!IsWallClimbing && !IsWallRunning)
	{
		// Determining whether player character is running forward in order to set the camera component's field of view accordingly
		mValue > 0.0f ? IsRunningForward = true : IsRunningForward = false;

		AddMovementInput(GetActorForwardVector(), mValue);
	}
}

void ATimeTravelController::MoveRight(float mValue)
{
	if (!IsWallClimbing && !IsWallRunning)
	{
		AddMovementInput(GetActorRightVector(), mValue);
	}
}

void ATimeTravelController::MoveCameraHorizontal(float mAmount)
{
	if (!IsWallClimbing)
	{
		AddControllerYawInput(mAmount);
	}
}

void ATimeTravelController::MoveCameraVertical(float mAmount)
{
	if (!IsWallClimbing)
	{
		AddControllerPitchInput(mAmount);
	}
}

// Handles the recall ability
void ATimeTravelController::RecallUpdate(const float & mDeltaTime)
{
	RemoveRecallTransforms();

	if (RecallTransforms.Num() > 0 && IsRecallActive)
	{
		// Dropping an object if it cannot be held during recall
		if (IsHoldingObject)
		{
			if (ObjectPickUpPhysicsHandle->GetGrabbedComponent()->GetOwner()->IsA(AAccessKey::StaticClass()))
			{
				AAccessKey* HeldKey = Cast<AAccessKey>(ObjectPickUpPhysicsHandle->GetGrabbedComponent()->GetOwner());

				if (HeldKey->DroppedOnRecall)
				{
					DropObject();
				}
			}
			else if (ObjectPickUpPhysicsHandle->GetGrabbedComponent()->GetOwner()->IsA(APortableObject::StaticClass()))
			{
				APortableObject* HeldObject = Cast<APortableObject>(ObjectPickUpPhysicsHandle->GetGrabbedComponent()->GetOwner());

				if (HeldObject->DroppedOnRecall)
				{
					DropObject();
				}
			}
		}

		SetActorLocation(UKismetMathLibrary::TLerp(GetActorTransform(), RecallTransforms[RecallTransformCounter], RecallLocationSpeed * mDeltaTime).GetLocation());

		// Rotation is disabled by default for now
		if (EnableRecallRotation)
		{
			GetController()->SetControlRotation(UKismetMathLibrary::RLerp(GetActorRotation(), RecallTransforms[RecallTransformCounter].GetRotation().Rotator(), RecallRotationSpeed * mDeltaTime, false));
		}

		if (UKismetMathLibrary::NearlyEqual_TransformTransform(GetActorTransform(), RecallTransforms[RecallTransformCounter], RecallTolerance, RecallTolerance, RecallTolerance))
		{
			// Stop recall and remove only the used elements from RecallTransformsArray
			if (RecallStopped)
			{
				for (int16 i = RecallTransforms.Num() - 1; i >= RecallTransformCounter; i--)
				{
					if (EnableDebug && EnableRecallDebug)
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

				if (EnableDebug && EnableRecallDebug)
				{
					UE_LOG(LogTemp, Error, TEXT("Cleared RecallTransforms."));
				}
			}
		}
	}
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
		if (EnableDebug && EnableRecallDebug)
		{
			UE_LOG(LogTemp, Error, TEXT("RecallTransforms is empty."));
		}
	}
}

void ATimeTravelController::AddRecallTransformToArray()
{
	// Only do this while recall is inactive
	if (CanSetPosition)
	{
		// Only add elements to RecallTransforms if it is empty or if the location has actually changed
		if (RecallTransforms.Num() == 0)
		{
			RecallTransforms.Add(GetActorTransform());
		}
		else if (!RecallTransforms.Last().GetLocation().Equals(GetActorTransform().GetLocation()))
		{
			RecallTransforms.Add(GetActorTransform());

			if (EnableDebug && EnableRecallDebug)
			{
				UE_LOG(LogTemp, Warning, TEXT("Added: %s"), *GetActorTransform().ToString());
			}
		}
	}
}

// Removes elements from RecallTransforms if the number of elements is above a certain limit
void ATimeTravelController::RemoveRecallTransforms()
{
	if (CanSetPosition)
	{
		if (RecallTransforms.Num() >= MaxStoredRecallTransforms)
		{
			RecallTransforms.RemoveAt(0);

			if (EnableDebug && EnableRecallDebug)
			{
				UE_LOG(LogTemp, Warning, TEXT("Removed: %s"), *RecallTransforms[0].ToString());
			}
		}
	}
}

void ATimeTravelController::ResetRecallCooldown()
{
	CanRecall = true;
}

void ATimeTravelController::WallClimbUpdate()
{
	if (GetWorld()->LineTraceSingleByChannel(WallClimbHitResult, GetActorLocation(), WallClimbLineTraceEnd->GetComponentLocation(), ECollisionChannel::ECC_Visibility))
	{
		if (WallClimbHitResult.GetActor()->ActorHasTag("Traversable"))
		{
			ClimbTime = GetWorld()->GetFirstPlayerController()->GetInputKeyTimeDown(FKey("SpaceBar"));

			if (EnableDebug && EnableWallClimbDebug)
			{
				UE_LOG(LogTemp, Warning, TEXT("ClimbTime: %f"), ClimbTime);
			}

			if (ClimbTime != 0.0f)
			{
				if (ClimbTime <= ClimbTimeMax)
				{
					LaunchCharacter(FVector(0.0f, 0.0f, ClimbSpeed), true, true);
				}
			}
		}
	}
	else
	{
		ClimbTime = 0.0f;
	}

	ClimbTime > 0.0f ? IsWallClimbing = true : IsWallClimbing = false;
}

void ATimeTravelController::ObjectPickUpUpdate()
{
	if (IsHoldingObject)
	{
		ObjectPickUpPhysicsHandle->SetTargetLocation(ObjectPickUpLineTraceEnd->GetComponentLocation());
	}
}

void ATimeTravelController::HandleObjectPickUpInput()
{
	if (IsHoldingObject)
	{
		DropObject();
	}
	else
	{
		PickUpObject();
	}
}

void ATimeTravelController::PickUpObject()
{
	if (GetWorld()->LineTraceSingleByChannel(ObjectPickUpHitResult, Camera->GetComponentLocation(), ObjectPickUpLineTraceEnd->GetComponentLocation(), ECollisionChannel::ECC_Visibility))
	{
		if (ObjectPickUpHitResult.GetComponent()->IsSimulatingPhysics())
		{
			ObjectPickUpPhysicsHandle->GrabComponentAtLocationWithRotation(
				ObjectPickUpHitResult.GetComponent(),
				NAME_None,
				ObjectPickUpHitResult.GetComponent()->GetComponentLocation(),
				ObjectPickUpHitResult.GetComponent()->GetComponentRotation()
			);

			IsHoldingObject = true;
		}
	}
}

void ATimeTravelController::DropObject()
{
	ObjectPickUpPhysicsHandle->ReleaseComponent();
	IsHoldingObject = false;
}

void ATimeTravelController::OnWallRunCollisionBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	JumpCount = 0;
	WallRunDirection = Camera->GetForwardVector();

	if (OtherActor->ActorHasTag("Traversable") && GetCharacterMovement()->IsFalling())
	{
		IsWallRunning = true;
	}
}

void ATimeTravelController::OnWallRunCollisionEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Traversable"))
	{
		WallRunEndReset();
	}
}

void ATimeTravelController::WallRunEndReset()
{
	GetCharacterMovement()->GravityScale = 1.0f;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 0.0f, 0.0f));
	IsWallRunning = false;
}

void ATimeTravelController::WallRunUpdate()
{
	if (!IsWallClimbing)
	{
		if (GetWorld()->GetFirstPlayerController()->GetInputKeyTimeDown(FKey("SpaceBar")) > 0.0f)
		{
			if (IsWallRunning)
			{
				GetCharacterMovement()->GravityScale = 0.0f;
				GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 0.0f, 1.0f));
				GetCharacterMovement()->AddForce(WallRunDirection * WallRunSpeed);
			}
			else
			{
				WallRunEndReset();
			}
		}
		else
		{
			WallRunEndReset();
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