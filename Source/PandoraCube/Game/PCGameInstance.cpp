// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/PCAudioSaveGame.h"

void UPCGameInstance::Init()
{
    Super::Init();
    LoadAudioSettings(); 
}

void UPCGameInstance::SaveAudioSettings()
{
    UPCAudioSaveGame* SaveGameInstance = Cast<UPCAudioSaveGame>(UGameplayStatics::CreateSaveGameObject(UPCAudioSaveGame::StaticClass()));
    if (SaveGameInstance)
    {
        SaveGameInstance->MasterVolume = MasterVolume;
        SaveGameInstance->BGMVolume = BGMVolume;
        SaveGameInstance->SFXVolume = SFXVolume;

        UGameplayStatics::SaveGameToSlot(SaveGameInstance, "AudioSettings", 0);
    }
}

void UPCGameInstance::LoadAudioSettings()
{
    UPCAudioSaveGame* SaveGameInstance = Cast<UPCAudioSaveGame>(UGameplayStatics::LoadGameFromSlot("AudioSettings", 0));
    if (SaveGameInstance)
    {
        MasterVolume = SaveGameInstance->MasterVolume;
        BGMVolume = SaveGameInstance->BGMVolume;
        SFXVolume = SaveGameInstance->SFXVolume;
    }
}

void UPCGameInstance::SetDifficulty(EDifficultyLevel NewDifficulty)
{
    SelectedDifficulty = NewDifficulty;
}

EDifficultyLevel UPCGameInstance::GetDifficulty()
{
    return SelectedDifficulty;
}

float UPCGameInstance::GetDifficultyMultiplier(EDifficultyLevel Difficulty)
{
    switch (Difficulty)
    {
        case EDifficultyLevel::EASY:
            return EasyMultiplier;
        case EDifficultyLevel::NORMAL:
            return NormalMultiplier;
        case EDifficultyLevel::HARD:
            return HardMultiplier;
        default:
            return 1.0f;
    }
}
