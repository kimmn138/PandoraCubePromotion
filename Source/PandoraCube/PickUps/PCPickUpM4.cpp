// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUps/PCPickUpM4.h"

APCPickUpM4::APCPickUpM4()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4'"));
	if (GunMeshRef.Object)
	{
		GunMesh->SetSkeletalMesh(GunMeshRef.Object);
	}
}
