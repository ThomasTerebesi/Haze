// Fill out your copyright notice in the Description page of Project Settings.


#include "GateTrigger.h"

#include "AccessKey.h"
#include "GateWithTriggers.h"
#include "PortableObject.h"
#include "TimeTravelController.h"


// Sets default values
AGateTrigger::AGateTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");

	RootComponent = SceneComponent;
	StaticMesh->SetupAttachment(SceneComponent);
	TriggerBox->SetupAttachment(StaticMesh);

	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGateTrigger::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AGateTrigger::OnOverlapEnd);

	TriggeredByObjectOverlapOnly = false;
	TriggeredByAccessKeyOverlapOnly = false;
	TriggeredByPlayerOverlapOnly = false;
	TriggeredByAnyOverlap = false;
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
	if (TriggeredByAccessKeyOverlapOnly)
	{
		if (OtherActor->IsA(AAccessKey::StaticClass()))
		{
			IsActivated = true;
		}
	}
	else if (TriggeredByObjectOverlapOnly)
	{
		if (OtherActor->IsA(APortableObject::StaticClass()))
		{
			IsActivated = true;
		}
	}
	else if (TriggeredByPlayerOverlapOnly)
	{
		if (OtherActor->IsA(ATimeTravelController::StaticClass()))
		{
			IsActivated = true;
		}
	}
	else if (TriggeredByAnyOverlap)
	{
		IsActivated = true;
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
	if (TriggeredByAccessKeyOverlapOnly)
	{
		if (OtherActor->IsA(AAccessKey::StaticClass()))
		{
			IsActivated = false;
		}
	}
	else if (TriggeredByObjectOverlapOnly)
	{
		if (OtherActor->IsA(APortableObject::StaticClass()))
		{
			IsActivated = false;
		}
	}
	else if (TriggeredByPlayerOverlapOnly)
	{
		if (OtherActor->IsA(ATimeTravelController::StaticClass()))
		{
			IsActivated = false;
		}
	}
	else if (TriggeredByAnyOverlap)
	{
		IsActivated = false;
	}
	else if (TriggeredByObjectOverlapOnly)
	{
		IsActivated = false;
	}
}

