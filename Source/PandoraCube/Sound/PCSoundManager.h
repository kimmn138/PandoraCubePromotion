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

    void SetMasterVolume(float Volume);
    void SetBGMVolume(float Volume);
    void SetSFXVolume(float Volume);

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TObjectPtr<class USoundClass> MasterSoundClass;

    UPROPERTY()
    TObjectPtr<class USoundClass> BGMSoundClass;

    UPROPERTY()
    TObjectPtr<class USoundClass> SFXSoundClass;

    void InitializeSoundClasses();
};
