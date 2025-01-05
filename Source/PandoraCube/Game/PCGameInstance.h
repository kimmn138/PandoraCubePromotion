// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Sound/PCSoundManager.h"
#include "PCMotionBlurManager.h"
#include "PCGameInstance.generated.h"


UENUM(BlueprintType)
enum class EDifficultyLevel : uint8
{
    EASY     UMETA(DisplayName = "Easy"),
    NORMAL   UMETA(DisplayName = "Normal"),
    HARD     UMETA(DisplayName = "Hard")
};
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

    float MouseSensitivity = 1.0f;

    bool bIsMotionBlurEnabled = true;

    void SaveAudioSettings();
    void LoadAudioSettings();

    void SaveMouseSettings();
    void LoadMouseSettings();

    void SaveMotionSettings();
    void LoadMotionSettings();

    void SetSoundManager(APCSoundManager* InSoundManager) { SoundManager = InSoundManager; }
    TObjectPtr<class APCSoundManager> GetSoundManager() const { return SoundManager; }

    void SetMotionBlurManager(APCMotionBlurManager* InMotionBlurManager) { MotionBlurManager = InMotionBlurManager; }
    TObjectPtr<class APCMotionBlurManager> GetMotionBlurManager() const { return MotionBlurManager; }

    UFUNCTION(BlueprintCallable, Category = "Game Time")
    void SetElapsedTime(const FString& Time) { ElapsedTime = Time; }
    UFUNCTION(BlueprintCallable, Category = "Game Time")
    FString GetElapsedTime() const { return ElapsedTime; }

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
    EDifficultyLevel SelectedDifficulty = EDifficultyLevel::NORMAL;

    UFUNCTION(BlueprintCallable)
    void SetDifficulty(EDifficultyLevel NewDifficulty);

    UFUNCTION(BlueprintCallable)
    float GetMouseSensitivity();

    UFUNCTION(BlueprintCallable)
    EDifficultyLevel GetDifficulty();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DifficultyModifiers")
    float EasyMultiplier = 0.6f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DifficultyModifiers")
    float NormalMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DifficultyModifiers")
    float HardMultiplier = 1.5f;

    float GetDifficultyMultiplier(EDifficultyLevel SelectedDifficulty);

private:
    UPROPERTY()
    TObjectPtr<class APCSoundManager> SoundManager;

    UPROPERTY()
    TObjectPtr<class APCMotionBlurManager> MotionBlurManager;

    UPROPERTY()
    FString ElapsedTime;
};
