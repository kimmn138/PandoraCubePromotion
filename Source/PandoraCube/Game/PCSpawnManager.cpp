// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCSpawnManager.h"
#include "AI/PCAISpawnLocation.h"
#include "Kismet/GameplayStatics.h"

APCSpawnManager* APCSpawnManager::Instance = nullptr;

APCSpawnManager* APCSpawnManager::GetInstance(UWorld* World)
{
    if (!Instance)
    {
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(World, APCSpawnManager::StaticClass(), FoundActors);
        if (FoundActors.Num() > 0)
        {
            Instance = Cast<APCSpawnManager>(FoundActors[0]);
        }
        else
        {
            Instance = World->SpawnActor<APCSpawnManager>();
        }
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

int32 APCSpawnManager::GetSpawnCountBasedOnDifficulty(FString Difficulty)
{
    if (Difficulty == "Easy")
    {
        return SpawnData.EasySpawnCount;
    }
    else if (Difficulty == "Medium")
    {
        return SpawnData.MediumSpawnCount;
    }
    else if (Difficulty == "Hard")
    {
        return SpawnData.HardSpawnCount;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Unknown Difficulty Level: %s"), *Difficulty);
        return 0;
    }
}
