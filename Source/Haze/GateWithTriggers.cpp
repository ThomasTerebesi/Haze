// Fill out your copyright notice in the Description page of Project Settings.

#include "GateWithTriggers.h"

// Sets default values
AGateWithTriggers::AGateWithTriggers()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");

	DebugEnabled = true;
}

// Called when the game starts or when spawned
void AGateWithTriggers::BeginPlay()
{
	Super::BeginPlay();
	
	NumberOfTriggers = TriggerArray.Num();

	if (DebugEnabled && NumberOfTriggers == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has no triggers attached to it!"), *GetName());
	}
}

// Called every frame
void AGateWithTriggers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

