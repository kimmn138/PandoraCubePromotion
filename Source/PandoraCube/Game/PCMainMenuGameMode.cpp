// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCMainMenuGameMode.h"
#include "Blueprint/UserWidget.h"
#include "PCGameInstance.h"
#include "Sound/PCSoundManager.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

APCMainMenuGameMode::APCMainMenuGameMode()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidgetBPClass(TEXT("/Game/PandoraCube/Blueprints/Widget/MainMenuWidget.MainMenuWidget_C"));
	if (MainMenuWidgetBPClass.Class)
	{
		MainMenuWidgetClass = MainMenuWidgetBPClass.Class;
	}

	DefaultPawnClass = nullptr;

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/PandoraCube.PCMenuController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> MainMenuBGMSoundRef(TEXT("/Game/PandoraCube/Sound/Cue/MainMenu_Cue.MainMenu_Cue"));
	if (MainMenuBGMSoundRef.Object)
	{
		MainMenuBGM = MainMenuBGMSoundRef.Object;
	}
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
