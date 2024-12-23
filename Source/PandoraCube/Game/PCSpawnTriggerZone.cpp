// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCSpawnTriggerZone.h"
#include "PCSpawnManager.h"
#include "Components/BoxComponent.h"
#include "Character/PCPlayerCharacter.h"

APCSpawnTriggerZone::APCSpawnTriggerZone()
{
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;
    TriggerBox->SetGenerateOverlapEvents(true);

    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APCSpawnTriggerZone::OnOverlapBegin);
}

void APCSpawnTriggerZone::BeginPlay()
{
    Super::BeginPlay();

    if (!SpawnManager || !SpawnManager->IsValidLowLevelFast())
    {
        SpawnManager = APCSpawnManager::GetInstance(GetWorld());
    }

    if (!SpawnManager)
    {
        return;
    }

    if (LinkedSpawnLocations.Num() > 0)
    {
        SpawnManager->RegisterSpawnLocationsForTriggerZone(this, LinkedSpawnLocations);
    }
}

void APCSpawnTriggerZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (APCPlayerCharacter* Player = Cast<APCPlayerCharacter>(OtherActor))
    {
        if (SpawnManager)
        {
            SpawnManager->ActivateSpawnLocations(LinkedSpawnLocations);
        }
    }
}

void APCSpawnTriggerZone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    if (SpawnManager)
    {
        SpawnManager = nullptr;
    }
}
