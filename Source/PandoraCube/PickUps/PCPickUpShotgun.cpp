// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUps/PCPickUpShotgun.h"

APCPickUpShotgun::APCPickUpShotgun()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/MilitaryWeapSilver/Weapons/Shotgun_A.Shotgun_A'"));
	if (GunMeshRef.Object)
	{
		GunMesh->SetSkeletalMesh(GunMeshRef.Object);
	}
}
