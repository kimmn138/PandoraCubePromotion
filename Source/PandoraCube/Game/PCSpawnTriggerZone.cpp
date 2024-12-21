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
        SpawnManager = APCSpawnManager::GetInstance(GetWorld());
    }

    if (SpawnManager)
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

void APCSpawnTriggerZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (APCPlayerCharacter* Player = Cast<APCPlayerCharacter>(OtherActor))
    {
        if (SpawnManager)
        {
            SpawnManager->InActivateSpawnLocations();
        }
    }
}
