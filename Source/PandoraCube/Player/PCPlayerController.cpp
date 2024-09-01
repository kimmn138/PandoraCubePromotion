// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PCPlayerController.h"

void APCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 뷰포트 안으로 마우스를 넣음
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
