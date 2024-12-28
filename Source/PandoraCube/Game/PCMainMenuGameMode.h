// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PCMainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PANDORACUBE_API APCMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APCMainMenuGameMode();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> MainMenuWidgetClass;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<class USoundBase> MainMenuBGM;

	void PlayMainMenuBGM();

private:
	UPROPERTY()
	TObjectPtr<class APCSoundManager> SoundManager;
};
