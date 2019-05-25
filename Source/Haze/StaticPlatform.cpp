// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticPlatform.h"

// Sets default values
AStaticPlatform::AStaticPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");

	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void AStaticPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStaticPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

