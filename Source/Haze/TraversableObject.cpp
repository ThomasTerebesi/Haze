// Fill out your copyright notice in the Description page of Project Settings.


#include "TraversableObject.h"

// Sets default values
ATraversableObject::ATraversableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");

	RootComponent = StaticMesh;

	IsTraversable = true;
	Tag = "Traversable";
}

// Called when the game starts or when spawned
void ATraversableObject::BeginPlay()
{
	Super::BeginPlay();

	if (IsTraversable)
	{
		Tags.Add(Tag);
	}
}

// Called every frame
void ATraversableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

