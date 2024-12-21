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

    float OffsetRange = 100.f;  
    FVector RandomOffset = FVector(FMath::RandRange(-OffsetRange, OffsetRange), FMath::RandRange(-OffsetRange, OffsetRange), 0.f);

    return SpawnLocation + RandomOffset;
}

void APCAISpawnLocation::BeginPlay()
{
    Super::BeginPlay();
}

