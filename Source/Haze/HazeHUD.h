// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CanvasItem.h"
#include "TextureResource.h"

#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"

#include "GameFramework/HUD.h"

#include "UObject/ConstructorHelpers.h"

#include "HazeHUD.generated.h"

/**
 * 
 */
UCLASS()
class HAZE_API AHazeHUD : public AHUD
{
	GENERATED_BODY()
	
//public:
//	AHazeHUD();
//
//protected:
//	virtual void BeginPlay() override;
//
//	virtual void DrawHUD() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UTexture2D* CrosshairTexture;
};
