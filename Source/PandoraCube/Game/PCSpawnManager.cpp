// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCSpawnManager.h"
#include "AI/PCAISpawnLocation.h"
#include "Kismet/GameplayStatics.h"

APCSpawnManager* APCSpawnManager::Instance = nullptr;

APCSpawnManager* APCSpawnManager::GetInstance(UWorld* World)
{
    if (!Instance)
    {
        Instance = World->SpawnActor<APCSpawnManager>();
    }
    return Instance;
}

void APCSpawnManager::ActivateSpawnLocations(const TArray<AActor*>& NewSpawnLocations)
{
    ActiveSpawnLocations = NewSpawnLocations;
}

void APCSpawnManager::SpawnZombiesInWave()
{
    for (AActor* SpawnLocationActor : ActiveSpawnLocations)
    {
        if (APCAISpawnLocation* Location = Cast<APCAISpawnLocation>(SpawnLocationActor))
        {
            if (ZombieClass)
            {
                GetWorld()->SpawnActor<APCCommonZombieCharacter>(ZombieClass, Location->GetActorLocation(), FRotator::ZeroRotator);
            }
        }
    }
}

void APCSpawnManager::RegisterSpawnLocationsForTriggerZone(AActor* TriggerZone, const TArray<AActor*>& SpawnLocations)
{
    if (TriggerZone)
    {
        TriggerZoneToSpawnLocationsMap.Add(TriggerZone, SpawnLocations);
    }
}
