// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUps/PCPickUpAK.h"

APCPickUpAK::APCPickUpAK()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/KA74U/SK_KA74U_X.SK_KA74U_X'"));
	if (GunMeshRef.Object)
	{
		GunMesh->SetSkeletalMesh(GunMeshRef.Object);
	}
}
