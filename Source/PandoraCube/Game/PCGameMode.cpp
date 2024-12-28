// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCGameMode.h"
#include "PCGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Sound/PCSoundManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/AudioComponent.h"
#include "PCSpawnManager.h"

APCGameMode::APCGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/PandoraCube/Blueprints/BP_PCPlayerCharacter.BP_PCPlayerCharacter_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/PandoraCube.PCPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

    static ConstructorHelpers::FObjectFinder<USoundBase> BackgroundMusicRef(TEXT("/Game/PandoraCube/Sound/Cue/InGame_Cue.InGame_Cue"));
    if (BackgroundMusicRef.Object)
    {
        BackgroundMusic = BackgroundMusicRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundBase> WaveMusicRef(TEXT("/Game/PandoraCube/Sound/Cue/InWave_Cue.InWave_Cue"));
    if (WaveMusicRef.Object)
    {
        WaveMusic = WaveMusicRef.Object;
    }
}

void APCGameMode::PlayBackgroundMusic()
{
    if (SoundManager && BackgroundMusic)
    {
        SoundManager->PlayBGM(BackgroundMusic);
    }
}

void APCGameMode::PlayWaveMusic()
{
    if (SoundManager && WaveMusic)
    {
        SoundManager->PlayBGM(WaveMusic);
    }
}

void APCGameMode::BeginPlay()
{
    Super::BeginPlay();

    APCSpawnManager* SpawnManager = APCSpawnManager::GetInstance(GetWorld());
    if (SpawnManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnManager Singleton found. Binding OnAllZombiesDead event."));

        if (!SpawnManager->OnAllZombiesDead.IsBound())
        {
            SpawnManager->OnAllZombiesDead.RemoveAll(this);
            SpawnManager->OnAllZombiesDead.AddDynamic(this, &APCGameMode::OnAllZombiesDead);
            UE_LOG(LogTemp, Warning, TEXT("OnAllZombiesDead event successfully bound in APCGameMode."));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("OnAllZombiesDead event was already bound in APCGameMode."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnManager Singleton not found!"));
    }

    UPCGameInstance* GameInstance = Cast<UPCGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        SoundManager = GetWorld()->SpawnActor<APCSoundManager>(
            APCSoundManager::StaticClass(),
            FVector::ZeroVector,
            FRotator::ZeroRotator
        );

        if (SoundManager)
        {
            SoundManager->SetMasterVolume(GameInstance->MasterVolume);
            SoundManager->SetBGMVolume(GameInstance->BGMVolume);
            SoundManager->SetSFXVolume(GameInstance->SFXVolume);

            GameInstance->SetSoundManager(SoundManager);

            PlayBackgroundMusic();
        }
    }
}

void APCGameMode::OnAllZombiesDead()
{
    UE_LOG(LogTemp, Warning, TEXT("All zombies are dead. Switching back to Background Music."));
    PlayBackgroundMusic();
}
