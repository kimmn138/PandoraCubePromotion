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
    return Instance;;
}

void APCSpawnManager::ActivateSpawnLocations(const TArray<AActor*>& NewSpawnLocations)
{
    if (NewSpawnLocations.Num() == 0)
    {
        return;
    }

    ActiveSpawnLocations = NewSpawnLocations;
}

void APCSpawnManager::InActivateSpawnLocations()
{
    ActiveSpawnLocations.Empty();
}

void APCSpawnManager::SpawnZombiesInWave()
{
    UE_LOG(LogTemp, Warning, TEXT("SpawnZombiesInWave: Started"));

    SpawnCount = GetSpawnCountBasedOnDifficulty(CurrentDifficulty);
    UE_LOG(LogTemp, Warning, TEXT("SpawnZombiesInWave: Spawn count based on difficulty: %d"), SpawnCount);

    if (!ZombieClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnZombiesInWave: ZombieClass is not set. Aborting spawn."));
        return;
    }

    if (ActiveSpawnLocations.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnZombiesInWave: No active spawn locations available."));
        return;
    }

    CurrentSpawnIndex = 0;

    GetWorldTimerManager().SetTimer(
        ZombieSpawnTimerHandle,
        this,
        &APCSpawnManager::SpawnZombie,
        0.5f,
        true
    );
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
        SpawnCount = SpawnData.EasySpawnCount;
    }
    else if (Difficulty == "Medium")
    {
        SpawnCount = SpawnData.MediumSpawnCount;
    }
    else if (Difficulty == "Hard")
    {
        SpawnCount = SpawnData.HardSpawnCount;
    }

    return SpawnCount;
}

void APCSpawnManager::SpawnZombie()
{
    if (SpawnCount <= 0 || ActiveSpawnLocations.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnZombie: All zombies have been spawned or no active locations remain."));
        GetWorldTimerManager().ClearTimer(ZombieSpawnTimerHandle); 
        return;
    }

    AActor* SpawnLocationActor = ActiveSpawnLocations[CurrentSpawnIndex];
    CurrentSpawnIndex = (CurrentSpawnIndex + 1) % ActiveSpawnLocations.Num();

    if (APCAISpawnLocation* Location = Cast<APCAISpawnLocation>(SpawnLocationActor))
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnZombie: Spawning zombie at location - %s"), *Location->GetName());

        FVector SpawnLocation = Location->GetSpawnLocation();

        APCCommonZombieCharacter* SpawnedZombie = GetWorld()->SpawnActor<APCCommonZombieCharacter>(
            ZombieClass, SpawnLocation, FRotator::ZeroRotator
        );

        if (SpawnedZombie)
        {
            UE_LOG(LogTemp, Warning, TEXT("SpawnZombie: Zombie successfully spawned - %s"), *SpawnedZombie->GetName());
            SpawnedZombie->SetImmediateChase(true);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("SpawnZombie: Failed to spawn zombie!"));
        }

        SpawnCount--;
        UE_LOG(LogTemp, Warning, TEXT("SpawnZombie: Remaining spawn count: %d"), SpawnCount);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnZombie: Invalid spawn location actor or failed to cast to APCAISpawnLocation"));
    }

    if (SpawnCount <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnZombie: All zombies spawned. Stopping timer."));
        GetWorldTimerManager().ClearTimer(ZombieSpawnTimerHandle);
    }
}
