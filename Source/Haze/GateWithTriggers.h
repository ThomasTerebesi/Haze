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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DebugEnabled;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsActivated;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 NumberOfTriggers;

	UFUNCTION(BlueprintCallable)
	bool RegisterTrigger(AGateTrigger* mTrigger);

	UFUNCTION(BlueprintCallable)
	bool AllGateTriggersAreActive();
};
