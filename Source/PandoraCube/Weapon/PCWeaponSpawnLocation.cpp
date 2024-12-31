// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PCWeaponSpawnLocation.h"
#include "Kismet/GameplayStatics.h"
#include "PickUps/PCPickUpBase.h"

APCWeaponSpawnLocation::APCWeaponSpawnLocation()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APCWeaponSpawnLocation::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnRandomWeapon();
}

void APCWeaponSpawnLocation::SpawnRandomWeapon()
{
    if (WeaponClasses.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, WeaponClasses.Num() - 1);
        TSubclassOf<APCPickUpBase> SelectedWeaponClass = WeaponClasses[RandomIndex];

        if (SelectedWeaponClass)
        {
            FVector SpawnLocation = GetActorLocation();
            FRotator SpawnRotation = GetActorRotation();

            APCPickUpBase* SpawnedWeapon = GetWorld()->SpawnActor<APCPickUpBase>(SelectedWeaponClass, SpawnLocation, SpawnRotation);
        }
    }
}

