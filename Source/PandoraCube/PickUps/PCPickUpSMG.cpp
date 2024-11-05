// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUps/PCPickUpSMG.h"

APCPickUpSMG::APCPickUpSMG()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/SMG11/SK_SMG11_Y.SK_SMG11_Y'"));
	if (GunMeshRef.Object)
	{
		GunMesh->SetSkeletalMesh(GunMeshRef.Object);
	}

	FName RowName = FName(*FString::FromInt(5));
	FString ContextString = TEXT("Item Data Context");

	if (ItemDataTable)
	{
		FInventoryItem* Row = ItemDataTable->FindRow<FInventoryItem>(RowName, ContextString);

		if (Row)
		{
			Item.ID = 5;
			Item.CurrentBullets = Row->Stats.MagSize;
			Item.TotalBullets = Row->Stats.MagSize * 3;
			Item.ItemType = EItemType::Primary;
		}
	}
}
