// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCMainMenuGameMode.h"
#include "Blueprint/UserWidget.h"
#include "PCGameInstance.h"
#include "Sound/PCSoundManager.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PCMenuController.h"

APCMainMenuGameMode::APCMainMenuGameMode()
{
}

void APCMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuWidgetClass)
	{
		UUserWidget* MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
			APCMenuController* PC = Cast<APCMenuController>(GetWorld()->GetFirstPlayerController());
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(MainMenuWidget->TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputModeData);
			PC->bShowMouseCursor = true;
		}
	}

	UPCGameInstance* GameInstance = Cast<UPCGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		SoundManager = GetWorld()->SpawnActor<APCSoundManager>(APCSoundManager::StaticClass());
		if (SoundManager)
		{
			SoundManager->SetMasterVolume(GameInstance->MasterVolume);
			SoundManager->SetBGMVolume(GameInstance->BGMVolume);
			SoundManager->SetSFXVolume(GameInstance->SFXVolume);

			GameInstance->SetSoundManager(SoundManager);
		}
	}

	PlayMainMenuBGM();
}

void APCMainMenuGameMode::PlayMainMenuBGM()
{
	if (SoundManager && MainMenuBGM)
	{
		SoundManager->PlayBGM(MainMenuBGM);
	}
}
