// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PCPlayerController.h"

void APCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ����Ʈ ������ ���콺�� ����
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
