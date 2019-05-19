// Fill out your copyright notice in the Description page of Project Settings.

#include "GateWithTriggers.h"

// Sets default values
AGateWithTriggers::AGateWithTriggers()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");

	RootComponent = Mesh;

	DebugEnabled = true;
}

// Called when the game starts or when spawned
void AGateWithTriggers::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGateWithTriggers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DebugEnabled && AllGateTriggersAreActive())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is activated!"), *GetName());
	}
}

bool AGateWithTriggers::RegisterTrigger(AGateTrigger* mTrigger)
{
	if (mTrigger->IsValidLowLevelFast())
	{
		GateTriggerArray.Add(mTrigger);
		NumberOfTriggers = GateTriggerArray.Num();

		if (DebugEnabled)
		{
			UE_LOG(LogTemp, Warning, TEXT("Successfully registered %s with gate."), *mTrigger->GetName());
		}

		return true;
	}
	else
	{
		if (DebugEnabled)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not register %s with gate."), *mTrigger->GetName());
		}

		return false;
	}
}

bool AGateWithTriggers::AllGateTriggersAreActive()
{
	for (AGateTrigger* CurrentTrigger : GateTriggerArray)
	{
		if (!CurrentTrigger->IsActivated)
		{
			return false;
		}
	}

	return true;
}

