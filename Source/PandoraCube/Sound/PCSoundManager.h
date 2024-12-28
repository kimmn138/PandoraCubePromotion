// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCSoundManager.generated.h"

UCLASS()
class PANDORACUBE_API APCSoundManager : public AActor
{
	GENERATED_BODY()
	
public:
    APCSoundManager();

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayBGM(USoundBase* BGM);

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void SetMasterVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void SetBGMVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void SetSFXVolume(float Volume);

protected:
    virtual void BeginPlay() override;

    UPROPERTY()
    TObjectPtr<class UPCGameInstance> GameInstance;

    UPROPERTY()
    TObjectPtr<class UAudioComponent> BGMAudioComponent;

private:
    UPROPERTY()
    TObjectPtr<class USoundClass> MasterSoundClass;

    UPROPERTY()
    TObjectPtr<class USoundClass> BGMSoundClass;

    UPROPERTY()
    TObjectPtr<class USoundClass> SFXSoundClass;

    void InitializeSoundClasses();

};
