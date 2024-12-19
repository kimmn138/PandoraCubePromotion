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

private:
    UFUNCTION(BlueprintCallable)
    void OnMasterVolumeChanged(float Value);

    UFUNCTION(BlueprintCallable)
    void OnBGMVolumeChanged(float Value);

    UFUNCTION(BlueprintCallable)
    void OnSFXVolumeChanged(float Value);

    TObjectPtr<class UPCGameInstance> GameInstance;
};
