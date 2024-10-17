// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PCAISpawnLocation.h"
#include "Components/BillboardComponent.h"

// Sets default values
APCAISpawnLocation::APCAISpawnLocation()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
    BillboardComponent->SetupAttachment(RootComponent);
    BillboardComponent->bHiddenInGame = true;
}

FVector APCAISpawnLocation::GetSpawnLocation() const
{
    FVector SpawnLocation = GetActorLocation();

    // 랜덤 오프셋을 추가하여 동일한 위치에서 스폰되지 않도록 함
    float OffsetRange = 100.f;  // 랜덤 오프셋 범위
    FVector RandomOffset = FVector(FMath::RandRange(-OffsetRange, OffsetRange), FMath::RandRange(-OffsetRange, OffsetRange), 0.f);

    return SpawnLocation + RandomOffset;
}

void APCAISpawnLocation::BeginPlay()
{
    Super::BeginPlay();
}

