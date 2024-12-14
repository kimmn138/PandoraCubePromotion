// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCMainMenuGameMode.h"
#include "Blueprint/UserWidget.h"

APCMainMenuGameMode::APCMainMenuGameMode()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidgetBPClass(TEXT("/Game/PandoraCube/Blueprints/Widget/MainMenuWidget.MainMenuWidget_C"));
	if (MainMenuWidgetBPClass.Class)
	{
		MainMenuWidgetClass = MainMenuWidgetBPClass.Class;
	}

	DefaultPawnClass = nullptr;
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
}
