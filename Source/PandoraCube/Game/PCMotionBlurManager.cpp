// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCMotionBlurManager.h"
#include "Engine/PostProcessVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Game/PCGameInstance.h"
#include "EngineUtils.h"

// Sets default values
APCMotionBlurManager::APCMotionBlurManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsMotionBlurEnabled = true;
}

void APCMotionBlurManager::SetMotionBlurEnabled(bool bEnable)
{
	for (TActorIterator<APostProcessVolume> It(GetWorld()); It; ++It)
	{
		APostProcessVolume* PostProcessVolume = *It;

		if (PostProcessVolume)
		{
			UE_LOG(LogTemp, Warning, TEXT("PostProcessVolume Found: %s"), *PostProcessVolume->GetName());
		}
		if (PostProcessVolume && PostProcessVolume->bUnbound)
		{
			PostProcessVolume->Settings.bOverride_MotionBlurAmount = true;
			PostProcessVolume->Settings.MotionBlurAmount = bEnable ? 0.5f : 0.0f;

			PostProcessVolume->Settings.bOverride_MotionBlurMax = true;
			PostProcessVolume->Settings.MotionBlurMax = bEnable ? 5.0f : 0.0f;

			break; 
		}
	}

	UPCGameInstance* GameInstance = Cast<UPCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->bIsMotionBlurEnabled = bEnable;
	}

	bIsMotionBlurEnabled = bEnable;
}

