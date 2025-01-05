// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PCSettingsWidget.h"
#include "Components/Slider.h"
#include "Kismet/GameplayStatics.h"
#include "Game/PCGameInstance.h"
#include "Components/CheckBox.h"

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

        if (MouseSensitivitySlider)
        {
            MouseSensitivitySlider->SetValue(GameInstance->MouseSensitivity);
            MouseSensitivitySlider->OnValueChanged.AddDynamic(this, &UPCSettingsWidget::OnMouseSensitivityChanged);
        }

        if (MotionBlurCheckBox)
        {
            MotionBlurCheckBox->SetIsChecked(GameInstance->bIsMotionBlurEnabled);
            MotionBlurCheckBox->OnCheckStateChanged.AddDynamic(this, &UPCSettingsWidget::OnMotionBlurCheckStateChanged);
        }
    }
}

void UPCSettingsWidget::OnMasterVolumeChanged(float Value)
{
    if (GameInstance)
    {
        GameInstance->MasterVolume = Value;
        GameInstance->SaveAudioSettings();

        if (GameInstance->GetSoundManager())
        {
            GameInstance->GetSoundManager()->SetMasterVolume(Value);
        }
    }
}

void UPCSettingsWidget::OnBGMVolumeChanged(float Value)
{
    if (GameInstance)
    {
        GameInstance->BGMVolume = Value;
        GameInstance->SaveAudioSettings();

        if (GameInstance->GetSoundManager())
        {
            GameInstance->GetSoundManager()->SetBGMVolume(Value);
        }
    }
}

void UPCSettingsWidget::OnSFXVolumeChanged(float Value)
{
    if (GameInstance)
    {
        GameInstance->SFXVolume = Value;
        GameInstance->SaveAudioSettings();

        if (GameInstance->GetSoundManager())
        {
            GameInstance->GetSoundManager()->SetSFXVolume(Value);
        }
    }
}

void UPCSettingsWidget::OnMouseSensitivityChanged(float Value)
{
    if (GameInstance)
    {
        GameInstance->MouseSensitivity = Value;
        GameInstance->SaveMouseSettings();
    }
}

void UPCSettingsWidget::OnMotionBlurCheckStateChanged(bool bIsChecked)
{
    if (GameInstance)
    {
        GameInstance->bIsMotionBlurEnabled = bIsChecked;
        GameInstance->SaveMotionSettings();

        if (GameInstance->GetMotionBlurManager())
        {
            GameInstance->GetMotionBlurManager()->SetMotionBlurEnabled(bIsChecked);
        }
    }
}
