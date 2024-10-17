// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/PCCommonZombieCharacter.h"
#include "PCSpawnManager.generated.h"

USTRUCT(BlueprintType)
struct FSpawnData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Data")
    int32 EasySpawnCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Data")
    int32 MediumSpawnCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Data")
    int32 HardSpawnCount;
};


UCLASS()
class PANDORACUBE_API APCSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	static APCSpawnManager* GetInstance(UWorld* World);

    void ActivateSpawnLocations(const TArray<AActor*>& NewSpawnLocations);
    
    void SpawnZombiesInWave();

    void RegisterSpawnLocationsForTriggerZone(AActor* TriggerZone, const TArray<AActor*>& SpawnLocations);

    int32 GetSpawnCountBasedOnDifficulty(FString Difficulty);

protected:
    TArray<AActor*> ActiveSpawnLocations;
    TMap<AActor*, TArray<AActor*>> TriggerZoneToSpawnLocationsMap;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<class APCCommonZombieCharacter> ZombieClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Data")
    FSpawnData SpawnData;

    FString CurrentDifficulty = TEXT("Easy");

private:
    static APCSpawnManager* Instance;
};
