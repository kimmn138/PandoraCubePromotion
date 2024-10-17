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
        // 로그 추가: 스폰 매니저 인스턴스 할당 로그
        SpawnManager = APCSpawnManager::GetInstance(GetWorld());
        UE_LOG(LogTemp, Log, TEXT("APCSpawnTriggerZone::BeginPlay - SpawnManager assigned"));
    }

    if (SpawnManager)
    {
        // 로그 추가: 트리거 존에 연결된 스폰 로케이션이 스폰 매니저에 등록되었는지 확인
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
            // 스폰 로케이션 활성화
            SpawnManager->ActivateSpawnLocations(LinkedSpawnLocations);
            UE_LOG(LogTemp, Log, TEXT("APCSpawnTriggerZone::OnOverlapBegin - Player entered trigger zone, activating spawn locations"));

            // 웨이브 상황에서 좀비를 스폰하는 함수 호출 추가
            SpawnManager->SpawnZombiesInWave();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("APCSpawnTriggerZone::OnOverlapBegin - SpawnManager is null when player entered"));
        }
    }
}
