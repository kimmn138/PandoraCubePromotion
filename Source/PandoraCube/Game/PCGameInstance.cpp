// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/PCAudioSaveGame.h"
#include "Mouse/PCMouseSaveGame.h"
#include "GameFramework/GameUserSettings.h"
#include "Player/PCPlayerController.h"

void UPCGameInstance::Init()
{
    Super::Init();
    LoadAudioSettings(); 
    LoadMouseSettings();
    LoadMotionSettings();
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

void UPCGameInstance::SaveMouseSettings()
{
    UPCMouseSaveGame* SaveGameInstance = Cast<UPCMouseSaveGame>(UGameplayStatics::CreateSaveGameObject(UPCMouseSaveGame::StaticClass()));
    if (SaveGameInstance)
    {
        SaveGameInstance->MouseSensitivity = MouseSensitivity;

        UGameplayStatics::SaveGameToSlot(SaveGameInstance, "MouseSettings", 0);
    }
}

void UPCGameInstance::LoadMouseSettings()
{
    UPCMouseSaveGame* SaveGameInstance = Cast<UPCMouseSaveGame>(UGameplayStatics::LoadGameFromSlot("MouseSettings", 0));
    if (SaveGameInstance)
    {
        MouseSensitivity = SaveGameInstance->MouseSensitivity;
    }
}

void UPCGameInstance::SaveMotionSettings()
{
    UPCMouseSaveGame* SaveGameInstance = Cast<UPCMouseSaveGame>(UGameplayStatics::CreateSaveGameObject(UPCMouseSaveGame::StaticClass()));
    if (SaveGameInstance)
    {
        SaveGameInstance->bIsMotionBlurEnabled = bIsMotionBlurEnabled;

        UGameplayStatics::SaveGameToSlot(SaveGameInstance, "MouseSettings", 0);
    }
}

void UPCGameInstance::LoadMotionSettings()
{
    UPCMouseSaveGame* SaveGameInstance = Cast<UPCMouseSaveGame>(UGameplayStatics::LoadGameFromSlot("MouseSettings", 0));
    if (SaveGameInstance)
    {
        bIsMotionBlurEnabled = SaveGameInstance->bIsMotionBlurEnabled;
    }
}

void UPCGameInstance::SetDifficulty(EDifficultyLevel NewDifficulty)
{
    SelectedDifficulty = NewDifficulty;
}

float UPCGameInstance::GetMouseSensitivity()
{
    return MouseSensitivity;
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
