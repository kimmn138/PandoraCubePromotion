// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCSpawnTriggerZone.h"
#include "PCSpawnManager.h"
#include "Components/BoxComponent.h"
#include "Character/PCPlayerCharacter.h"

APCSpawnTriggerZone::APCSpawnTriggerZone()
{
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;

    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APCSpawnTriggerZone::OnOverlapBegin);

}

void APCSpawnTriggerZone::BeginPlay()
{
    Super::BeginPlay();

    if (!SpawnManager)
    {
        // �α� �߰�: ���� �Ŵ��� �ν��Ͻ� �Ҵ� �α�
        SpawnManager = APCSpawnManager::GetInstance(GetWorld());
        UE_LOG(LogTemp, Log, TEXT("APCSpawnTriggerZone::BeginPlay - SpawnManager assigned"));
    }

    if (SpawnManager)
    {
        // �α� �߰�: Ʈ���� ���� ����� ���� �����̼��� ���� �Ŵ����� ��ϵǾ����� Ȯ��
        SpawnManager->RegisterSpawnLocationsForTriggerZone(this, LinkedSpawnLocations);
        UE_LOG(LogTemp, Log, TEXT("APCSpawnTriggerZone::BeginPlay - Registered %d spawn locations to the spawn manager"), LinkedSpawnLocations.Num());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("APCSpawnTriggerZone::BeginPlay - SpawnManager is null"));
    }
}

void APCSpawnTriggerZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (APCPlayerCharacter* Player = Cast<APCPlayerCharacter>(OtherActor))
    {
        if (SpawnManager)
        {
            // ���� �����̼� Ȱ��ȭ
            SpawnManager->ActivateSpawnLocations(LinkedSpawnLocations);
            UE_LOG(LogTemp, Log, TEXT("APCSpawnTriggerZone::OnOverlapBegin - Player entered trigger zone, activating spawn locations"));

            // ���̺� ��Ȳ���� ���� �����ϴ� �Լ� ȣ�� �߰�
            SpawnManager->SpawnZombiesInWave();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("APCSpawnTriggerZone::OnOverlapBegin - SpawnManager is null when player entered"));
        }
    }
}
