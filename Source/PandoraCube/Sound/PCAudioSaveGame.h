// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PCAudioSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PANDORACUBE_API UPCAudioSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
    UPROPERTY(VisibleAnywhere, Category = "Sound")
    float MasterVolume = 1.0f;

    UPROPERTY(VisibleAnywhere, Category = "Sound")
    float BGMVolume = 1.0f;

    UPROPERTY(VisibleAnywhere, Category = "Sound")
    float SFXVolume = 1.0f;
};
