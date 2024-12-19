// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PCGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PANDORACUBE_API APCGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	APCGameMode();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<class APCSoundManager> SoundManager;
};
