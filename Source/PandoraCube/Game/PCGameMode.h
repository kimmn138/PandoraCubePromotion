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

	UFUNCTION(BlueprintCallable)
	void PlayBackgroundMusic();

	UFUNCTION(BlueprintCallable)
	void PlayWaveMusic();

	UFUNCTION()
	void OnAllZombiesDead();

	void StartGameTimer();
	void StopGameTimer();
	void PauseGameTimer();
	void ResumeGameTimer();
	FString GetElapsedTime() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Audio")
	TObjectPtr<class USoundBase> BackgroundMusic;

	UPROPERTY(EditAnywhere, Category = "Audio")
	TObjectPtr<class USoundBase> WaveMusic;

	float GameStartTime;
	float GameEndTime;
	float PauseStartTime;
	float TotalPausedTime;
	bool bIsGameRunning;
	bool bIsPaused;

private:
	UPROPERTY()
	TObjectPtr<class APCSoundManager> SoundManager;

	FTimerHandle WaveMusicTimerHandle;
};
