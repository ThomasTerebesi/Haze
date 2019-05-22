// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"

#include "GameFramework/Actor.h"

#include "AccessKey.h"
#include "GateWithTriggers.h"

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
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GateTrigger")
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GateTrigger")
	AGateWithTriggers* AssignedGate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GateTrigger")
	bool TriggeredByObjectOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GateTrigger")
	bool TriggeredByAccessKeyOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GateTrigger")
	bool MakesGateStayOpen;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GateTrigger")
	bool IsActivated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool DebugEnabled;

	UFUNCTION(BlueprintCallable)
	bool RegisterWithAssignedGate();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
