// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/StaticMeshComponent.h"

#include "GameFramework/Actor.h"

#include "GateTrigger.h"

#include "GateWithTriggers.generated.h"


UCLASS()
class HAZE_API AGateWithTriggers : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGateWithTriggers();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AGateTrigger*> GateTriggerArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gate with Triggers")
	bool IsActivated;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gate with Triggers")
	int32 NumberOfTriggers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate with Triggers")
	FVector InitialLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate with Triggers")
	FVector CurrentLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate with Triggers")
	FVector OpenEndLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate with Triggers")
	float GateOpenSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate with Triggers")
	float GateCloseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate with Triggers")
	float AllGateTriggersActiveDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate with Triggers")
	bool ActivatesWithAnyTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate with Triggers")
	bool GateStaysOpen;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gate with Triggers")
	FName Tag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool DebugEnabled;

	UFUNCTION(BlueprintCallable)
	bool RegisterTrigger(AGateTrigger* mTrigger);

	UFUNCTION(BlueprintCallable)
	void AllGateTriggersAreActive();

private:
	FTimerHandle AllGateTriggersActiveDelayHandle;
};
