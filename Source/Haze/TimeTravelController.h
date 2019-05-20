// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AccessKey.h"

#include "Camera/CameraComponent.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SceneComponent.h"

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
	USceneComponent* ObjectPickUpLineTraceEnd;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPhysicsHandleComponent* ObjectPickUpPhysicsHandle;

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

	UFUNCTION(BlueprintCallable)
	void Recall();

	UFUNCTION(BlueprintCallable)
	void ResetRecallCooldown();

	UFUNCTION(BlueprintCallable)
	void PickUpObject();

	UFUNCTION(BlueprintCallable)
	void DropObject();


	// "Debug" category
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool EnableDebug;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool EnableRecallDebug;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool EnableWallClimbDebug;


	// "Recall" category
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


	// "Object Pick Up" category
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Object Pick Up")
	bool IsHoldingObject;


	// "Other" category
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 JumpMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	float StandardFieldOfView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	float ForwardFieldOfView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	float RecallFieldOfView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	float FieldOfViewSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Other")
	bool IsRunningForward;

private:
	void RemoveRecallTransforms();

	void SetFieldOfView(const float & mDeltaTime);

	void HandleRecall(const float & mDeltaTime);

	void HandleWallClimb();

	void UpdateHeldObject();

	void HandleObjectPickUpInput();

	void AddRecallTransformToArray();

	FTimerHandle StorePositionDelayHandle;

	FTimerHandle RecallCooldownHandle;

	FHitResult WallClimbHitResult;

	FHitResult ObjectPickUpHitResult;
};
