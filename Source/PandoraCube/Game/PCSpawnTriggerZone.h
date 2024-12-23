// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCSpawnTriggerZone.generated.h"

UCLASS()
class PANDORACUBE_API APCSpawnTriggerZone : public AActor
{
	GENERATED_BODY()
	
public:	
	APCSpawnTriggerZone();

protected:
    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UPROPERTY(EditAnywhere, Category = "Spawn")
    TArray<AActor*> LinkedSpawnLocations;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    TObjectPtr<class APCSpawnManager> SpawnManager;

    UPROPERTY(EditAnywhere)
    TObjectPtr<class UBoxComponent> TriggerBox;
};
