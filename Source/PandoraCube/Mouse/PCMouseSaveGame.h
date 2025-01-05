// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PCMouseSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PANDORACUBE_API UPCMouseSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = "Mouse")
	float MouseSensitivity = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = "Motion")
	bool bIsMotionBlurEnabled = true;
};
