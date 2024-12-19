// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PCSettingsWidget.h"
#include "Components/Slider.h"
#include "Kismet/GameplayStatics.h"
#include "Game/PCGameInstance.h"

void UPCSettingsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    GameInstance = Cast<UPCGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GameInstance)
    {
        if (MasterVolumeSlider)
        {
            MasterVolumeSlider->SetValue(GameInstance->MasterVolume);
            MasterVolumeSlider->OnValueChanged.AddDynamic(this, &UPCSettingsWidget::OnMasterVolumeChanged);
        }

        if (BGMVolumeSlider)
        {
            BGMVolumeSlider->SetValue(GameInstance->BGMVolume);
            BGMVolumeSlider->OnValueChanged.AddDynamic(this, &UPCSettingsWidget::OnBGMVolumeChanged);
        }

        if (SFXVolumeSlider)
        {
            SFXVolumeSlider->SetValue(GameInstance->SFXVolume);
            SFXVolumeSlider->OnValueChanged.AddDynamic(this, &UPCSettingsWidget::OnSFXVolumeChanged);
        }
    }
}

void UPCSettingsWidget::OnMasterVolumeChanged(float Value)
{
    if (GameInstance)
    {
        GameInstance->MasterVolume = Value;
        GameInstance->SaveAudioSettings();
    }
}

void UPCSettingsWidget::OnBGMVolumeChanged(float Value)
{
    if (GameInstance)
    {
        GameInstance->BGMVolume = Value;
        GameInstance->SaveAudioSettings();
    }
}

void UPCSettingsWidget::OnSFXVolumeChanged(float Value)
{
    if (GameInstance)
    {
        GameInstance->SFXVolume = Value;
        GameInstance->SaveAudioSettings();
    }
}
