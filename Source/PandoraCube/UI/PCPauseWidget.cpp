// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PCPauseWidget.h"
#include "Game/PCGameMode.h"
#include "Kismet/GameplayStatics.h"

void UPCPauseWidget::ResumeGame()
{
    UWorld* World = GetWorld();
    if (!World) return;

    APCGameMode* GameMode = Cast<APCGameMode>(UGameplayStatics::GetGameMode(World));
    if (GameMode)
    {
        GameMode->ResumeGameTimer();
    }
}
