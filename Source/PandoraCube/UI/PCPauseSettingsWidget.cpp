// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PCPauseSettingsWidget.h"
#include "Components/Slider.h"
#include "Kismet/GameplayStatics.h"
#include "Game/PCGameInstance.h"
#include "Character/PCPlayerCharacter.h"
#include "Player/PCPlayerController.h"
#include "Components/CheckBox.h"

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

        if (MouseSensitivitySlider)
        {
            MouseSensitivitySlider->SetValue(GameInstance->MouseSensitivity);
            MouseSensitivitySlider->OnValueChanged.AddDynamic(this, &UPCPauseSettingsWidget::OnMouseSensitivityChanged);
        }

        if (MotionBlurCheckBox)
        {
            MotionBlurCheckBox->SetIsChecked(GameInstance->bIsMotionBlurEnabled);
            MotionBlurCheckBox->OnCheckStateChanged.AddDynamic(this, &UPCPauseSettingsWidget::OnMotionBlurCheckStateChanged);
        }
    }
}

void UPCPauseSettingsWidget::OnMasterVolumeChanged(float Value)
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

void UPCPauseSettingsWidget::OnBGMVolumeChanged(float Value)
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

void UPCPauseSettingsWidget::OnSFXVolumeChanged(float Value)
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

void UPCPauseSettingsWidget::OnMouseSensitivityChanged(float Value)
{
    if (GameInstance)
    {
        GameInstance->MouseSensitivity = Value;
        GameInstance->SaveMouseSettings();
        
        APCPlayerController* PlayerController = Cast<APCPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

        if (PlayerController)
        {
            APCPlayerCharacter* PlayerCharacter = Cast<APCPlayerCharacter>(PlayerController->GetPawn());
            if (PlayerCharacter)
            {
                PlayerCharacter->SetMouseSensitivity(GameInstance->MouseSensitivity);
            }
        }
    }
}

void UPCPauseSettingsWidget::OnMotionBlurCheckStateChanged(bool bIsChecked)
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
