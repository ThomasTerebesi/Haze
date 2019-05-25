// Fill out your copyright notice in the Description page of Project Settings.


#include "PortableObject.h"

// Sets default values
APortableObject::APortableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");

	RootComponent = StaticMesh;

	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetGenerateOverlapEvents(true);
	StaticMesh->SetCollisionProfileName("PhysicsActor");
}

// Called when the game starts or when spawned
void APortableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APortableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

