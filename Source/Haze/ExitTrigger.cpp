// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitTrigger.h"

// Sets default values
AExitTrigger::AExitTrigger()
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
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AExitTrigger::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AExitTrigger::OnOverlapEnd);

	NextLevel = "L_One";
}

// Called when the game starts or when spawned
void AExitTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExitTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExitTrigger::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	GoToNextLevel();
}

void AExitTrigger::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	GoToNextLevel();
}

void AExitTrigger::GoToNextLevel()
{
	UGameplayStatics::OpenLevel(this, NextLevel, true);
}

