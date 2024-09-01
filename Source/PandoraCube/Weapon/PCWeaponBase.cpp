// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PCWeaponBase.h"

// Sets default values
APCWeaponBase::APCWeaponBase()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMesh;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/MilitaryWeapSilver/Weapons/Assault_Rifle_A.Assault_Rifle_A'"));
	if (SkeletalMeshMeshRef.Object)
	{
		SkeletalMesh->SetSkeletalMesh(SkeletalMeshMeshRef.Object);
	}
}

