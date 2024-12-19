// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCGameMode.h"
#include "PCGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Sound/PCSoundManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

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
}

void APCGameMode::BeginPlay()
{
    Super::BeginPlay();

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
        }
    }
}
