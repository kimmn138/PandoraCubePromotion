// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCSpawnManager.h"
#include "AI/PCAISpawnLocation.h"
#include "Kismet/GameplayStatics.h"

APCSpawnManager* APCSpawnManager::Instance = nullptr;

APCSpawnManager* APCSpawnManager::GetInstance(UWorld* World)
{
    if (!Instance)
    {
        // �α� �߰�: �ν��Ͻ��� �������� �ʾ��� �� �������� �˸�
        UE_LOG(LogTemp, Log, TEXT("APCSpawnManager::GetInstance - Creating a new instance of APCSpawnManager"));
        Instance = World->SpawnActor<APCSpawnManager>();
    }
    else
    {
        // �α� �߰�: �̹� �ν��Ͻ��� �������� �˸�
        UE_LOG(LogTemp, Log, TEXT("APCSpawnManager::GetInstance - Returning existing instance of APCSpawnManager"));
    }
    return Instance;;
}

void APCSpawnManager::ActivateSpawnLocations(const TArray<AActor*>& NewSpawnLocations)
{
    if (NewSpawnLocations.Num() == 0)
    {
        // �α� �߰�: Ȱ��ȭ�� ���� �����̼��� ���� ��� ���
        UE_LOG(LogTemp, Warning, TEXT("APCSpawnManager::ActivateSpawnLocations - No spawn locations provided"));
        return;
    }

    // �α� �߰�: Ȱ��ȭ�� ���� �����̼��� �� ���
    UE_LOG(LogTemp, Log, TEXT("APCSpawnManager::ActivateSpawnLocations - Activating %d spawn locations"), NewSpawnLocations.Num());

    ActiveSpawnLocations = NewSpawnLocations;
}

void APCSpawnManager::SpawnZombiesInWave()
{
    int32 SpawnCount = GetSpawnCountBasedOnDifficulty(CurrentDifficulty);

    // �α� �߰�: �����Ϸ��� ���� Ŭ������ �����Ǿ����� Ȯ��
    if (!ZombieClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("APCSpawnManager::SpawnZombiesInWave - ZombieClass is not set"));
        return;
    }

    // �α� �߰�: ���� ���̵� �� ������ ���� �� ���
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
                        // ���̺�� ��ȯ�� ������� ��� �߰�
                        SpawnedZombie->SetImmediateChase(true);
                    }
                    // �α� �߰�: ���� ������ ��ġ �� ���� ���� �� ���
                    UE_LOG(LogTemp, Log, TEXT("APCSpawnManager::SpawnZombiesInWave - Spawned a zombie at location: %s, Remaining Spawn Count: %d"), *Location->GetActorLocation().ToString(), SpawnCount - 1);

                    SpawnCount--;
                }
            }
        }
    }

    if (SpawnCount <= 0)
    {
        // �α� �߰�: ���� �Ϸ� �޽��� ���
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

    // �α� �߰�: ���̵��� ���� ���� �� ���
    UE_LOG(LogTemp, Log, TEXT("APCSpawnManager::GetSpawnCountBasedOnDifficulty - Difficulty: %s, Spawn Count: %d"), *Difficulty, SpawnCount);

    return SpawnCount;
}
