// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/PCSpawnManager.h"
#include "AI/PCAISpawnLocation.h"
#include "Kismet/GameplayStatics.h"
#include "PCGameMode.h"
#include "EngineUtils.h"

APCSpawnManager* APCSpawnManager::Instance = nullptr;

APCSpawnManager* APCSpawnManager::GetInstance(UWorld* World)
{
    if (Instance && Instance->IsValidLowLevelFast())
    {
        return Instance;
    }

    if (World)
    {
        for (TActorIterator<APCSpawnManager> It(World); It; ++It)
        {
            Instance = *It;
            break;
        }

        if (!Instance)
        {
            Instance = World->SpawnActor<APCSpawnManager>();
            if (Instance)
            {
                Instance->AddToRoot();
            }
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
    if (GetWorld())
    {
        UPCGameInstance* GameInstance = Cast<UPCGameInstance>(GetWorld()->GetGameInstance());
        if (GameInstance)
        {
            CurrentDifficulty = GameInstance->GetDifficulty();
        }
    }

    SpawnCount = GetSpawnCountBasedOnDifficulty(CurrentDifficulty);

    if (SpawnCount <= 0)
    {
        return;
    }

    if (ZombieClasses.Num() == 0)
    {
        return;
    }

    if (ActiveSpawnLocations.Num() == 0)
    {
        return;
    }

    CurrentSpawnIndex = 0;
    AliveZombiesCount = 0;

    if (APCGameMode* GM = Cast<APCGameMode>(UGameplayStatics::GetGameMode(this)))
    {
        GM->PlayWaveMusic();
    }

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

int32 APCSpawnManager::GetSpawnCountBasedOnDifficulty(EDifficultyLevel Difficulty)
{
    int32 SCount = 0;

    switch (Difficulty)
    {
    case EDifficultyLevel::EASY:
        return SpawnData.EasySpawnCount;
    case EDifficultyLevel::NORMAL:
        return SpawnData.MediumSpawnCount;
    case EDifficultyLevel::HARD:
        return SpawnData.HardSpawnCount;
    default:
        return SpawnData.MediumSpawnCount;
    }
}

void APCSpawnManager::SpawnZombie()
{
    if (SpawnCount <= 0 || ActiveSpawnLocations.Num() == 0 || ZombieClasses.Num() == 0)
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

        TSubclassOf<APCEnemyCharacterBase> SelectedZombieClass = GetRandomZombieClassByDifficulty(CurrentDifficulty);
        if (!SelectedZombieClass)
        {
            return;
        }

        APCEnemyCharacterBase* SpawnedZombie = GetWorld()->SpawnActor<APCEnemyCharacterBase>(SelectedZombieClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

        if (SpawnedZombie)
        {
            SpawnedZombie->SetImmediateChase(true);
            SpawnedZombie->OnZombieDeath.AddDynamic(this, &APCSpawnManager::OnZombieDeath);

            AliveZombiesCount++;
        }

        SpawnCount--;
    }

    if (SpawnCount <= 0)
    {
        GetWorldTimerManager().ClearTimer(ZombieSpawnTimerHandle);
    }
}

void APCSpawnManager::OnZombieDeath()
{
    AliveZombiesCount--;

    if (AliveZombiesCount <= 0)
    {
        if (OnAllZombiesDead.IsBound())
        {
            OnAllZombiesDead.Broadcast();
        }
        else
        {
            APCGameMode* GameMode = Cast<APCGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
            if (GameMode)
            {
                if (!OnAllZombiesDead.IsBound())
                {
                    OnAllZombiesDead.RemoveAll(GameMode);
                    OnAllZombiesDead.AddDynamic(GameMode, &APCGameMode::OnAllZombiesDead);
                }
                OnAllZombiesDead.Broadcast();
            }
        }
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

TSubclassOf<APCEnemyCharacterBase> APCSpawnManager::GetRandomZombieClassByDifficulty(EDifficultyLevel Difficulty)
{
    if (ZombieClasses.Num() < 3)
    {
        return nullptr;
    }

    TArray<int32> ProbabilityRanges;

    switch (Difficulty)
    {
    case EDifficultyLevel::EASY:
        ProbabilityRanges = { 12, 18, 20 }; 
        break;
    case EDifficultyLevel::NORMAL:
        ProbabilityRanges = { 10, 17, 20 }; 
        break;
    case EDifficultyLevel::HARD:
        ProbabilityRanges = { 8, 15, 20 };
        break;
    default:
        ProbabilityRanges = { 12, 18, 20 }; 
        break;
    }

    int32 RandomValue = FMath::RandRange(1, 20);
    
    if (RandomValue <= ProbabilityRanges[0])
    {
        return ZombieClasses[0]; 
    }
    else if (RandomValue <= ProbabilityRanges[1])
    {
        return ZombieClasses[1]; 
    }
    else if (RandomValue <= ProbabilityRanges[2])
    {
        return ZombieClasses[2];
    }

    return ZombieClasses[0];
}
