// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCSpawnManager.h"
#include "AI/PCAISpawnLocation.h"
#include "Kismet/GameplayStatics.h"

APCSpawnManager* APCSpawnManager::Instance = nullptr;

APCSpawnManager* APCSpawnManager::GetInstance(UWorld* World)
{
    if (!Instance)
    {
        // 로그 추가: 인스턴스가 생성되지 않았을 때 생성됨을 알림
        UE_LOG(LogTemp, Log, TEXT("APCSpawnManager::GetInstance - Creating a new instance of APCSpawnManager"));
        Instance = World->SpawnActor<APCSpawnManager>();
    }
    else
    {
        // 로그 추가: 이미 인스턴스가 존재함을 알림
        UE_LOG(LogTemp, Log, TEXT("APCSpawnManager::GetInstance - Returning existing instance of APCSpawnManager"));
    }
    return Instance;;
}

void APCSpawnManager::ActivateSpawnLocations(const TArray<AActor*>& NewSpawnLocations)
{
    if (NewSpawnLocations.Num() == 0)
    {
        // 로그 추가: 활성화할 스폰 로케이션이 없는 경우 경고
        UE_LOG(LogTemp, Warning, TEXT("APCSpawnManager::ActivateSpawnLocations - No spawn locations provided"));
        return;
    }

    // 로그 추가: 활성화된 스폰 로케이션의 수 출력
    UE_LOG(LogTemp, Log, TEXT("APCSpawnManager::ActivateSpawnLocations - Activating %d spawn locations"), NewSpawnLocations.Num());

    ActiveSpawnLocations = NewSpawnLocations;
}

void APCSpawnManager::SpawnZombiesInWave()
{
    int32 SpawnCount = GetSpawnCountBasedOnDifficulty(CurrentDifficulty);

    // 로그 추가: 스폰하려는 좀비 클래스가 설정되었는지 확인
    if (!ZombieClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("APCSpawnManager::SpawnZombiesInWave - ZombieClass is not set"));
        return;
    }

    // 로그 추가: 현재 난이도 및 스폰할 좀비 수 출력
    UE_LOG(LogTemp, Log, TEXT("APCSpawnManager::SpawnZombiesInWave - Difficulty: %s, Spawn Count: %d"), *CurrentDifficulty, SpawnCount);

    while (SpawnCount > 0 && ActiveSpawnLocations.Num() > 0)
    {
        for (AActor* SpawnLocationActor : ActiveSpawnLocations)
        {
            if (APCAISpawnLocation* Location = Cast<APCAISpawnLocation>(SpawnLocationActor))
            {
                if (ZombieClass && SpawnCount > 0)
                {
                    APCCommonZombieCharacter* SpawnedZombie = GetWorld()->SpawnActor<APCCommonZombieCharacter>(ZombieClass, Location->GetSpawnLocation(), FRotator::ZeroRotator);


                    if (SpawnedZombie)
                    {
                        // 웨이브로 소환된 좀비들은 즉시 추격
                        SpawnedZombie->SetImmediateChase(true);
                    }
                    // 로그 추가: 좀비가 스폰된 위치 및 남은 스폰 수 출력
                    UE_LOG(LogTemp, Log, TEXT("APCSpawnManager::SpawnZombiesInWave - Spawned a zombie at location: %s, Remaining Spawn Count: %d"), *Location->GetActorLocation().ToString(), SpawnCount - 1);

                    SpawnCount--;
                }
            }
        }
    }

    if (SpawnCount <= 0)
    {
        // 로그 추가: 스폰 완료 메시지 출력
        UE_LOG(LogTemp, Log, TEXT("APCSpawnManager::SpawnZombiesInWave - All zombies spawned"));
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
    int32 SpawnCount = 0;

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
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("APCSpawnManager::GetSpawnCountBasedOnDifficulty - Unknown Difficulty Level: %s"), *Difficulty);
    }

    // 로그 추가: 난이도에 따른 스폰 수 출력
    UE_LOG(LogTemp, Log, TEXT("APCSpawnManager::GetSpawnCountBasedOnDifficulty - Difficulty: %s, Spawn Count: %d"), *Difficulty, SpawnCount);

    return SpawnCount;
}
