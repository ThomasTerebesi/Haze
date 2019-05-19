// Fill out your copyright notice in the Description page of Project Settings.

#include "AccessKey.h"

// Sets default values
AAccessKey::AAccessKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	RootComponent = Mesh;

	DroppedOnRecall = false;

	Mesh->SetSimulatePhysics(true);
	Mesh->SetMassOverrideInKg(NAME_None, 200.0f, true);
}

// Called when the game starts or when spawned
void AAccessKey::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAccessKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

