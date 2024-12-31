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
        }

        GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &APCGeometryCollectionBox::DestroyActor, 5.0f, false);
    }
}

void APCGeometryCollectionBox::DestroyActor()
{
    Destroy();
}
