// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PCPlayerController.h"
#include "Game/PCGameInstance.h"
#include "Kismet/GameplayStatics.h"

void APCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
