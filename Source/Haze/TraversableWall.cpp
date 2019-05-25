// Fill out your copyright notice in the Description page of Project Settings.


#include "TraversableWall.h"

// Sets default values
ATraversableWall::ATraversableWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");

	RootComponent = StaticMesh;

	IsTraversable = true;
	Tag = "Traversable";
}

// Called when the game starts or when spawned
void ATraversableWall::BeginPlay()
{
	Super::BeginPlay();

	if (IsTraversable)
	{
		Tags.Add(Tag);
	}
}

// Called every frame
void ATraversableWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

