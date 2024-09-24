// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUps/PCPickUpSMG.h"

APCPickUpSMG::APCPickUpSMG()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/SMG11/SK_SMG11_Y.SK_SMG11_Y'"));
	if (GunMeshRef.Object)
	{
		GunMesh->SetSkeletalMesh(GunMeshRef.Object);
	}
}
