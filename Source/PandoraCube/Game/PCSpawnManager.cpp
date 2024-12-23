// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/PCSpawnManager.h"
#include "AI/PCAISpawnLocation.h"
#include "Kismet/GameplayStatics.h"

APCSpawnManager* APCSpawnManager::Instance = nullptr;

APCSpawnManager* APCSpawnManager::GetInstance(UWorld* World)
{
    if (!Instance || !Instance->IsValidLowLevelFast())
    {
        if (!World)
        {
            return nullptr;
        }

        Instance = World->SpawnActor<APCSpawnManager>();
        if (Instance)
        {
            Instance->AddToRoot();
        }
    }

    return Instance;
}

void APCSpawnManager::ActivateSpawnLocations(const TArray<AActor*>& NewSpawnLocations)
{
    if (NewSpawnLocations.Num() == 0)
    {
        return;
    }

    ActiveSpawnLocations.Empty();

    ActiveSpawnLocations = NewSpawnLocations;
    CurrentSpawnIndex = 0;
}

void APCSpawnManager::SpawnZombiesInWave()
{
    SpawnCount = GetSpawnCountBasedOnDifficulty(CurrentDifficulty);

    if (SpawnCount <= 0)
    {
        return;
    }

    if (!ZombieClass)
    {
        return;
    }

    if (ActiveSpawnLocations.Num() == 0)
    {
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
    int32 SCount = 0;

    if (Difficulty == "Easy")
    {
        SCount = SpawnData.EasySpawnCount;
    }
    else if (Difficulty == "Medium")
    {
        SCount = SpawnData.MediumSpawnCount;
    }
    else if (Difficulty == "Hard")
    {
        SCount = SpawnData.HardSpawnCount;
    }

    return SCount;
}

void APCSpawnManager::SpawnZombie()
{
    if (SpawnCount <= 0 || ActiveSpawnLocations.Num() == 0)
    {
        GetWorldTimerManager().ClearTimer(ZombieSpawnTimerHandle);
        return;
    }

    if (CurrentSpawnIndex >= ActiveSpawnLocations.Num())
    {
        CurrentSpawnIndex = 0;
    }

    AActor* SpawnLocationActor = ActiveSpawnLocations[CurrentSpawnIndex];
    CurrentSpawnIndex = (CurrentSpawnIndex + 1) % ActiveSpawnLocations.Num();

    if (APCAISpawnLocation* Location = Cast<APCAISpawnLocation>(SpawnLocationActor))
    {
        FVector SpawnLocation = Location->GetSpawnLocation();

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        SpawnParams.bNoFail = true;

        APCCommonZombieCharacter* SpawnedZombie = GetWorld()->SpawnActor<APCCommonZombieCharacter>(
            ZombieClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

        if (SpawnedZombie)
        {
            SpawnedZombie->SetImmediateChase(true);
        }

        SpawnCount--;
    }

    if (SpawnCount <= 0)
    {
        GetWorldTimerManager().ClearTimer(ZombieSpawnTimerHandle);
    }
}

void APCSpawnManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    if (Instance)
    {
        Instance->RemoveFromRoot();
        Instance = nullptr;
    }
}
