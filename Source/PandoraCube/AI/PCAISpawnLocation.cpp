// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PCAISpawnLocation.h"

// Sets default values
APCAISpawnLocation::APCAISpawnLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APCAISpawnLocation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APCAISpawnLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

