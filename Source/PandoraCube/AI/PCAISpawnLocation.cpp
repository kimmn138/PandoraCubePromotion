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
    return GetActorLocation();
}

void APCAISpawnLocation::BeginPlay()
{
    Super::BeginPlay();
}

