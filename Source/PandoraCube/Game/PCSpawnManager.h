// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/PCCommonZombieCharacter.h"
#include "Game/PCGameInstance.h"
#include "PCSpawnManager.generated.h"

USTRUCT(BlueprintType)
struct FSpawnData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Data")
    int32 EasySpawnCount = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Data")
    int32 MediumSpawnCount = 15;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Data")
    int32 HardSpawnCount = 20;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllZombiesDead);

UCLASS()
class PANDORACUBE_API APCSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
    UFUNCTION(BlueprintCallable, Category = "Singleton")
	static APCSpawnManager* GetInstance(UWorld* World);

    void ActivateSpawnLocations(const TArray<AActor*>& NewSpawnLocations);
    
    void SpawnZombiesInWave();

    void RegisterSpawnLocationsForTriggerZone(AActor* TriggerZone, const TArray<AActor*>& SpawnLocations);

    int32 GetSpawnCountBasedOnDifficulty(EDifficultyLevel Difficulty);

    UPROPERTY(BlueprintAssignable, Category = "SpawnManager")
    FOnAllZombiesDead OnAllZombiesDead;

protected:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    TArray<AActor*> ActiveSpawnLocations;
    TMap<AActor*, TArray<AActor*>> TriggerZoneToSpawnLocationsMap;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<class APCCommonZombieCharacter> ZombieClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Data")
    FSpawnData SpawnData;

    EDifficultyLevel CurrentDifficulty;

    UPROPERTY()
    int32 AliveZombiesCount = 0;

    UFUNCTION()
    void SpawnZombie();

    UFUNCTION()
    void OnZombieDeath();

private:
    static APCSpawnManager* Instance;

private:
    int32 SpawnCount;
    FTimerHandle ZombieSpawnTimerHandle; 
    int32 CurrentSpawnIndex;
};
