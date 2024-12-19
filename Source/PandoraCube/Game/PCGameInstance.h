// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Sound/PCSoundManager.h"
#include "PCGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PANDORACUBE_API UPCGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    virtual void Init() override;

    float MasterVolume = 1.0f;
    float BGMVolume = 1.0f;
    float SFXVolume = 1.0f;

    void SaveAudioSettings();
    void LoadAudioSettings();

    void SetSoundManager(APCSoundManager* InSoundManager) { SoundManager = InSoundManager; }
    TObjectPtr<class APCSoundManager> GetSoundManager() const { return SoundManager; }

private:
    UPROPERTY()
    TObjectPtr<class APCSoundManager> SoundManager;
};
