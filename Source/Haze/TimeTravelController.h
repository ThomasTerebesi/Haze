// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AccessKey.h"

#include "Camera/CameraComponent.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SceneComponent.h"
#include "Components/TimelineComponent.h"

#include "DrawDebugHelpers.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "TimeTravelController.generated.h"

UCLASS()
class HAZE_API ATimeTravelController : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATimeTravelController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// Components and functions
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* WallClimbLineTraceEnd;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* WallRunCollision; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* WallRunLeftCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* WallRunRightCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* ObjectPickUpLineTraceEnd;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPhysicsHandleComponent* ObjectPickUpPhysicsHandle;


	// "Debug" category
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool EnableDebug;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool EnableRecallDebug;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool EnableWallClimbDebug;


	// "Recall" category
	UFUNCTION(BlueprintCallable)
	void Recall();

	UFUNCTION(BlueprintCallable)
	void ResetRecallCooldown();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recall")
	bool CanSetPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recall")
	float StorePositionDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recall")
	int32 MaxStoredRecallTransforms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recall")
	float RecallLocationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recall")
	float RecallRotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recall")
	float RecallCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recall")
	float RecallTolerance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Recall")
	bool CanRecall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recall")
	bool EnableRecallRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Recall")
	bool IsRecallActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Recall")
	bool RecallStopped;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Recall")
	int32 RecallTransformCounter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Recall")
	TArray<FTransform> RecallTransforms;


	// "Wall Climb" category
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Climb")
	bool EnableWallClimb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Climb")
	float ClimbTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Climb")
	float ClimbTimeMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Climb")
	float ClimbSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wall Climb")
	bool IsClimbing;

	
	// "Wall Run" category
	UFUNCTION(BlueprintCallable)
	void OnCharacterLanded(const FHitResult & mHit);

	UFUNCTION(BlueprintCallable)
	void OnWallRunCollisionBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnWallRunLeftCollision(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnWallRunRightCollision(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnWallRunCollisionEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void WallRunRotationUpdate(float mDeltaTime);

	UFUNCTION(BlueprintCallable)
	void WallRunUpdate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WallRunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WallRunRotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WallRunRotationAmount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wall Run")
	FVector WallRunDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wall Run")
	bool IsWallRunning;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wall Run")
	bool WallIsLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wall Run")
	bool WallIsRight;


	// "Object Pick Up" category
	UFUNCTION(BlueprintCallable)
	void PickUpObject();

	UFUNCTION(BlueprintCallable)
	void DropObject();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Object Pick Up")
	bool IsHoldingObject;


	// "General" category
	void Jump();

	void StopJump();

	UFUNCTION(BlueprintCallable)
	void MoveForward(float mValue);

	UFUNCTION(BlueprintCallable)
	void MoveRight(float mValue);

	UFUNCTION(BlueprintCallable)
	void MoveCameraHorizontal(float mAmount);

	UFUNCTION(BlueprintCallable)
	void MoveCameraVertical(float mAmount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxJumps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpLaunchVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General")
	int32 JumpCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	float StandardFieldOfView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	float ForwardFieldOfView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	float RecallFieldOfView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	float FieldOfViewSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General")
	bool IsRunningForward;


private:
	void RemoveRecallTransforms();

	void SetFieldOfView(const float & mDeltaTime);

	void RecallUpdate(const float & mDeltaTime);

	void WallClimbUpdate();

	void ObjectPickUpUpdate();

	void HandleObjectPickUpInput();

	void AddRecallTransformToArray();

	void WallRunEndReset();

	FTimerHandle StorePositionDelayHandle;

	FTimerHandle RecallCooldownHandle;

	FHitResult WallClimbHitResult;

	FHitResult ObjectPickUpHitResult;
};
