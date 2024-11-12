// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCWeaponSpawnLocation.generated.h"

UCLASS()
class PANDORACUBE_API APCWeaponSpawnLocation : public AActor
{
	GENERATED_BODY()
	
public:	
	APCWeaponSpawnLocation();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TArray<TSubclassOf<class APCPickUpBase>> WeaponClasses;

	void SpawnRandomWeapon();
};
