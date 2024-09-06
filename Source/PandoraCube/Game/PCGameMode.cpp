// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCGameMode.h"

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
