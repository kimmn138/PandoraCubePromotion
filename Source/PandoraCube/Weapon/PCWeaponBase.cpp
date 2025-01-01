// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PCWeaponBase.h"
#include "Components/SceneComponent.h"

// Sets default values
APCWeaponBase::APCWeaponBase()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->SetRelativeRotation(FRotator(-1.399858f, 0.0f, 0.0f));

	AimOffset = CreateDefaultSubobject<USceneComponent>(TEXT("AimOffset"));
	AimOffset->SetupAttachment(RootComponent);
}

