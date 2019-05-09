// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"

#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"

#include "DrawDebugHelpers.h"

#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"

#include "Kismet/KismetMathLibrary.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;

	void Jump();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool EnableDebug;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool ShowRecallTransforms;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Recall")
	bool RecallPressed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Recall")
	bool RecallStopped;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Recall")
	int32 RecallTransformCounter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Recall")
	TArray<FTransform> RecallTransforms;

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
	void AddRecallTransform();

	FTimerHandle StorePositionDelayHandle;

	FTimerHandle RecallCooldownHandle;
};
