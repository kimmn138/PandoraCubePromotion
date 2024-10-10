// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCAISpawnLocation.generated.h"

UCLASS()
class PANDORACUBE_API APCAISpawnLocation : public AActor
{
	GENERATED_BODY()
	
public:	
    APCAISpawnLocation();

    FVector GetSpawnLocation() const;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, Category = "Spawn")
    TObjectPtr<class UBillboardComponent> BillboardComponent;

};
