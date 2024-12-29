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

    GameStartTime = 0.0f;
    GameEndTime = 0.0f;
    PauseStartTime = 0.0f;
    TotalPausedTime = 0.0f;
    bIsGameRunning = false;
    bIsPaused = false;
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
        if (!SpawnManager->OnAllZombiesDead.IsBound())
        {
            SpawnManager->OnAllZombiesDead.RemoveAll(this);
            SpawnManager->OnAllZombiesDead.AddDynamic(this, &APCGameMode::OnAllZombiesDead);
        }
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

    StartGameTimer();
}

void APCGameMode::OnAllZombiesDead()
{
    PlayBackgroundMusic();
}

void APCGameMode::StartGameTimer()
{
    GameStartTime = GetWorld()->GetTimeSeconds();
    TotalPausedTime = 0.0f;
    bIsGameRunning = true;
    bIsPaused = false;
}

void APCGameMode::StopGameTimer()
{
    if (bIsGameRunning)
    {
        GameEndTime = GetWorld()->GetTimeSeconds();
        bIsGameRunning = false;

        float ElapsedTime = GameEndTime - GameStartTime - TotalPausedTime;
    }
}

void APCGameMode::PauseGameTimer()
{
    if (bIsGameRunning && !bIsPaused)
    {
        PauseStartTime = GetWorld()->GetTimeSeconds();
        bIsPaused = true;
    }
}

void APCGameMode::ResumeGameTimer()
{
    if (bIsPaused)
    {
        float PauseEndTime = GetWorld()->GetTimeSeconds();
        TotalPausedTime += PauseEndTime - PauseStartTime;
        bIsPaused = false;
    }
}

FString APCGameMode::GetElapsedTime() const
{
    float ElapsedTime = bIsGameRunning ? GetWorld()->GetTimeSeconds() - GameStartTime - TotalPausedTime : GameEndTime - GameStartTime - TotalPausedTime;

    int32 Minutes = FMath::FloorToInt(ElapsedTime / 60.0f);
    int32 Seconds = FMath::FloorToInt(FMath::Fmod(ElapsedTime, 60.0f));

    return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
}
