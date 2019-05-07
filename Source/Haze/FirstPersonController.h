// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"

#include "Components/InputComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"

#include "FirstPersonController.generated.h"

UCLASS()
class HAZE_API AFirstPersonController : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFirstPersonController();

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
};
