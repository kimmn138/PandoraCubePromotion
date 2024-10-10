// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/PCCommonZombieCharacter.h"
#include "PCSpawnManager.generated.h"

UCLASS()
class PANDORACUBE_API APCSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	static APCSpawnManager* GetInstance(UWorld* World);

    void ActivateSpawnLocations(const TArray<AActor*>& NewSpawnLocations);
    
    void SpawnZombiesInWave();

    void RegisterSpawnLocationsForTriggerZone(AActor* TriggerZone, const TArray<AActor*>& SpawnLocations);

protected:
    TArray<AActor*> ActiveSpawnLocations;
    TMap<AActor*, TArray<AActor*>> TriggerZoneToSpawnLocationsMap;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<class APCCommonZombieCharacter> ZombieClass;

private:
    static APCSpawnManager* Instance;
};
