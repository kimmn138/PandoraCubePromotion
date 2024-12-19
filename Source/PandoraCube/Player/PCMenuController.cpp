// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PCMenuController.h"

void APCMenuController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeUIOnly UIOnlyInputMode;
	SetInputMode(UIOnlyInputMode);
}
