// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "PCGeometryCollectionBox.generated.h"

/**
 * 
 */
UCLASS()
class PANDORACUBE_API APCGeometryCollectionBox : public AGeometryCollectionActor
{
	GENERATED_BODY()

public:
    APCGeometryCollectionBox();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class UGeometryCollectionComponent> GeometryCollectionComp;

    bool bIsDestroyed;

    FTimerHandle DestroyTimerHandle;

    UFUNCTION()
    void OnGeometryBreak(const FChaosBreakEvent& Event);

    UFUNCTION()
    void DestroyActor();
};
