// Fill out your copyright notice in the Description page of Project Settings.


#include "GateTrigger.h"

// Sets default values
AGateTrigger::AGateTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");

	RootComponent = Mesh;
	TriggerBox->SetupAttachment(Mesh);

	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGateTrigger::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AGateTrigger::OnOverlapEnd);

	TriggeredByObjectOverlap = false;
	TriggeredByAccessKeyOverlap = false;
	MakesGateStayOpen = false;
	IsActivated = false;

	DebugEnabled = true;
}

// Called when the game starts or when spawned
void AGateTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	RegisterWithAssignedGates();
}

// Called every frame
void AGateTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGateTrigger::RegisterWithAssignedGates()
{
	for (AGateWithTriggers* Gate : AssignedGates)
	{
		if (Gate->IsValidLowLevelFast())
		{
			Gate->RegisterTrigger(this);
		}
	}
}

void AGateTrigger::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (TriggeredByAccessKeyOverlap)
	{
		if (OtherActor->IsA(AAccessKey::StaticClass()))
		{
			IsActivated = true;
		}

		if (DebugEnabled)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s successfully triggered %s."), *GetName(), *OtherActor->GetName());
		}
	}
	else if (TriggeredByObjectOverlap)
	{
		IsActivated = true;

		if (DebugEnabled)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s successfully triggered %s."), *GetName(), *OtherActor->GetName());
		}
	}

	if (MakesGateStayOpen)
	{
		for (AGateWithTriggers* Gate : AssignedGates)
		{
			Gate->GateStaysOpen = true;
		}
	}
}

void AGateTrigger::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (TriggeredByAccessKeyOverlap)
	{
		if (OtherActor->IsA(AAccessKey::StaticClass()))
		{
			IsActivated = false;

			if (DebugEnabled)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s not triggered by %s anymore."), *GetName(), *OtherActor->GetName());
			}
		}
	}
	else if (TriggeredByObjectOverlap)
	{
		IsActivated = false;

		if (DebugEnabled)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s not triggered by %s anymore."), *GetName(), *OtherActor->GetName());
		}
	}
}

