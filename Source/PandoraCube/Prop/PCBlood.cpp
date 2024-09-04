// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/PCBlood.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInstance.h"

// Sets default values
APCBlood::APCBlood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Blood = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	RootComponent = Blood;

	Blood->SetWorldRotation(FRotator(0.0f, 180.0f, 0.0f));
	Blood->SetWorldScale3D(FVector(0.086596f, 0.653236f, 0.653236f));

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BloodMaterialRef(TEXT("/Game/Megascans/Decals/High_Velocity_Blood_Spatter_sfjjcfzp/MI_High_Velocity_Blood_Spatter_sfjjcfzp_2K.MI_High_Velocity_Blood_Spatter_sfjjcfzp_2K"));
	if (nullptr != BloodMaterialRef.Object)
	{
		Blood->SetDecalMaterial(BloodMaterialRef.Object);
	}
}

// Called when the game starts or when spawned
void APCBlood::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APCBlood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

