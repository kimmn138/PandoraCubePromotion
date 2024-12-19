// Fill out your copyright notice in the Description page of Project Settings.


#include "Sound/PCSoundManager.h"
#include "Sound/SoundClass.h"
#include "Kismet/GameplayStatics.h"
#include "Game/PCGameInstance.h"

APCSoundManager::APCSoundManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void APCSoundManager::BeginPlay()
{
    Super::BeginPlay();

    InitializeSoundClasses();

    UPCGameInstance* GameInstance = Cast<UPCGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GameInstance)
    {
        SetMasterVolume(GameInstance->MasterVolume);
        SetBGMVolume(GameInstance->BGMVolume);
        SetSFXVolume(GameInstance->SFXVolume);
    }
}

void APCSoundManager::InitializeSoundClasses()
{
    MasterSoundClass = LoadObject<USoundClass>(nullptr, TEXT("/Script/Engine.SoundClass'/Game/PandoraCube/Sound/PCMasterSoundClass.PCMasterSoundClass'"));
    BGMSoundClass = LoadObject<USoundClass>(nullptr, TEXT("/Script/Engine.SoundClass'/Game/PandoraCube/Sound/PCBGMSoundClass.PCBGMSoundClass'"));
    SFXSoundClass = LoadObject<USoundClass>(nullptr, TEXT("/Script/Engine.SoundClass'/Game/PandoraCube/Sound/PCSFXSoundClass.PCSFXSoundClass'"));

    if (!MasterSoundClass || !BGMSoundClass || !SFXSoundClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Sound classes failed to load. Check your asset paths."));
    }
}

void APCSoundManager::SetMasterVolume(float Volume)
{
    if (MasterSoundClass)
    {
        MasterSoundClass->Properties.Volume = Volume;
    }
}

void APCSoundManager::SetBGMVolume(float Volume)
{
    if (BGMSoundClass)
    {
        BGMSoundClass->Properties.Volume = Volume;
    }
}

void APCSoundManager::SetSFXVolume(float Volume)
{
    if (SFXSoundClass)
    {
        SFXSoundClass->Properties.Volume = Volume;
    }
}
