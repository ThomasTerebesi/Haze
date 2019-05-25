// Fill out your copyright notice in the Description page of Project Settings.

#include "GateWithTriggers.h"

// Sets default values
AGateWithTriggers::AGateWithTriggers()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");

	RootComponent = Mesh;

	IsActivated = false;
	AllGateTriggersActiveDelay = 0.25f;
	GateOpenSpeed = 8.0f;
	GateCloseSpeed = 8.0f;
	ActivatesWithAnyTrigger = false;
	GateStaysOpen = false;

	DebugEnabled = true;

	OpenEndLocation = FVector(0.0f, 0.0f, 128.0f);
}

// Called when the game starts or when spawned
void AGateWithTriggers::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();

	GetWorld()->GetTimerManager().SetTimer(AllGateTriggersActiveDelayHandle, this, &AGateWithTriggers::AllGateTriggersAreActive, AllGateTriggersActiveDelay, true);
}

// Called every frame
void AGateWithTriggers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentLocation = GetActorLocation();

	if ((IsActivated || GateStaysOpen) && !CurrentLocation.Equals(InitialLocation + OpenEndLocation))
	{
		SetActorLocation(FMath::VInterpTo(CurrentLocation, InitialLocation + OpenEndLocation, DeltaTime, GateOpenSpeed));
	}
	else if (!GateStaysOpen && !IsActivated && !CurrentLocation.Equals(InitialLocation))
	{
		SetActorLocation(FMath::VInterpTo(CurrentLocation, InitialLocation, DeltaTime, GateCloseSpeed));
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

void AGateWithTriggers::AllGateTriggersAreActive()
{
	if (!ActivatesWithAnyTrigger)
	{
		for (AGateTrigger* CurrentTrigger : GateTriggerArray)
		{
			if (!CurrentTrigger->IsActivated)
			{
				IsActivated = false;
				return;
			}
		}

		IsActivated = true;
	}
	else
	{
		for (AGateTrigger* CurrentTrigger : GateTriggerArray)
		{
			if (CurrentTrigger->IsActivated)
			{
				IsActivated = true;
				return;
			}
		}

		IsActivated = false;
	}
}

