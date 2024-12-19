// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PCPauseSettingsWidget.h"
#include "Components/Slider.h"
#include "Kismet/GameplayStatics.h"
#include "Game/PCGameInstance.h"

void UPCPauseSettingsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    GameInstance = Cast<UPCGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GameInstance)
    {
        if (MasterVolumeSlider)
        {
            MasterVolumeSlider->SetValue(GameInstance->MasterVolume);
            MasterVolumeSlider->OnValueChanged.AddDynamic(this, &UPCPauseSettingsWidget::OnMasterVolumeChanged);
        }

        if (BGMVolumeSlider)
        {
            BGMVolumeSlider->SetValue(GameInstance->BGMVolume);
            BGMVolumeSlider->OnValueChanged.AddDynamic(this, &UPCPauseSettingsWidget::OnBGMVolumeChanged);
        }

        if (SFXVolumeSlider)
        {
            SFXVolumeSlider->SetValue(GameInstance->SFXVolume);
            SFXVolumeSlider->OnValueChanged.AddDynamic(this, &UPCPauseSettingsWidget::OnSFXVolumeChanged);
        }
    }
}

void UPCPauseSettingsWidget::OnMasterVolumeChanged(float Value)
{
    if (GameInstance)
    {
        GameInstance->MasterVolume = Value;
        GameInstance->SaveAudioSettings();
    }
}

void UPCPauseSettingsWidget::OnBGMVolumeChanged(float Value)
{
    if (GameInstance)
    {
        GameInstance->BGMVolume = Value;
        GameInstance->SaveAudioSettings();
    }
}

void UPCPauseSettingsWidget::OnSFXVolumeChanged(float Value)
{
    if (GameInstance)
    {
        GameInstance->SFXVolume = Value;
        GameInstance->SaveAudioSettings();
    }
}
