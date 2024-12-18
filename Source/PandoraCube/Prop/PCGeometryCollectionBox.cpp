// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/PCGeometryCollectionBox.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

APCGeometryCollectionBox::APCGeometryCollectionBox()
{
}

void APCGeometryCollectionBox::BeginPlay()
{
    Super::BeginPlay();

    GeometryCollectionComp = GetGeometryCollectionComponent();
    RootComponent = GeometryCollectionComp;

    if (GeometryCollectionComp)
    {
        GeometryCollectionComp->SetNotifyBreaks(true);

        GeometryCollectionComp->OnChaosBreakEvent.AddDynamic(this, &APCGeometryCollectionBox::OnGeometryBreak);

        UE_LOG(LogTemp, Warning, TEXT("BreakNotify has been enabled."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to initialize GeometryCollectionComp!"));
    }
}

void APCGeometryCollectionBox::OnGeometryBreak(const FChaosBreakEvent& Event)
{
    if (!bIsDestroyed)
    {
        bIsDestroyed = true;

        if (GeometryCollectionComp)
        {
            GeometryCollectionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            UE_LOG(LogTemp, Warning, TEXT("Geometry Collection Broken at Location: %s"), *Event.Location.ToString());
        }

        GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &APCGeometryCollectionBox::DestroyActor, 5.0f, false);
    }
}

void APCGeometryCollectionBox::DestroyActor()
{
    UE_LOG(LogTemp, Warning, TEXT("Actor Destroyed!"));
    Destroy();
}
