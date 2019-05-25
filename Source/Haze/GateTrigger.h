// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"

#include "GameFramework/Actor.h"

#include "GateTrigger.generated.h"

class AGateWithTriggers;

UCLASS()
class HAZE_API AGateTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGateTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GateTrigger")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GateTrigger")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GateTrigger")
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GateTrigger")
	TArray<AGateWithTriggers*> AssignedGates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GateTrigger")
	bool TriggeredByObjectOverlapOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GateTrigger")
	bool TriggeredByAccessKeyOverlapOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GateTrigger")
	bool TriggeredByPlayerOverlapOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GateTrigger")
	bool TriggeredByAnyOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GateTrigger")
	bool MakesGateStayOpen;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GateTrigger")
	bool IsActivated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool DebugEnabled;

	UFUNCTION(BlueprintCallable)
	void RegisterWithAssignedGates();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
