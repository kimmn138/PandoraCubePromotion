// Fill out your copyright notice in the Description page of Project Settings.


#include "Sound/PCSoundManager.h"
#include "Sound/SoundClass.h"
#include "Kismet/GameplayStatics.h"
#include "Game/PCGameInstance.h"
#include "Components/AudioComponent.h"

APCSoundManager::APCSoundManager()
{
    PrimaryActorTick.bCanEverTick = false;

    BGMAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMAudioComponent"));
    BGMAudioComponent->bAutoActivate = false; 
    BGMAudioComponent->bIsUISound = true; 
    RootComponent = BGMAudioComponent;
}

void APCSoundManager::BeginPlay()
{
    Super::BeginPlay();

    InitializeSoundClasses();

    GameInstance = Cast<UPCGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GameInstance)
    {
        SetMasterVolume(GameInstance->MasterVolume);
        SetBGMVolume(GameInstance->BGMVolume);
        SetSFXVolume(GameInstance->SFXVolume);
    }
}

void APCSoundManager::PlayBGM(USoundBase* BGM)
{
    if (BGM)
    {
        if (BGMAudioComponent->IsPlaying())
        {
            BGMAudioComponent->Stop();
        }

        BGMAudioComponent->SetSound(BGM);
        BGMAudioComponent->SetVolumeMultiplier(GameInstance ? GameInstance->BGMVolume : 1.0f);
        BGMAudioComponent->Play();
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

    if (BGMAudioComponent && BGMAudioComponent->IsPlaying())
    {
        BGMAudioComponent->SetVolumeMultiplier(BGMSoundClass->Properties.Volume);
    }
}

void APCSoundManager::SetSFXVolume(float Volume)
{
    if (SFXSoundClass)
    {
        SFXSoundClass->Properties.Volume = Volume;
    }
}
