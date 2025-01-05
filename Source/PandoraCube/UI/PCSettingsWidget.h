// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PCSettingsWidget.generated.h"

/**
 * 
 */
UCLASS()
class PANDORACUBE_API UPCSettingsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeOnInitialized() override;

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class USlider> MasterVolumeSlider;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class USlider> BGMVolumeSlider;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class USlider> SFXVolumeSlider;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class USlider> MouseSensitivitySlider;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UCheckBox> MotionBlurCheckBox;

private:
    UFUNCTION(BlueprintCallable)
    void OnMasterVolumeChanged(float Value);

    UFUNCTION(BlueprintCallable)
    void OnBGMVolumeChanged(float Value);

    UFUNCTION(BlueprintCallable)
    void OnSFXVolumeChanged(float Value);

    UFUNCTION(BlueprintCallable)
    void OnMouseSensitivityChanged(float Value);

    UFUNCTION(BlueprintCallable)
    void OnMotionBlurCheckStateChanged(bool bIsChecked);

    TObjectPtr<class UPCGameInstance> GameInstance;
};
