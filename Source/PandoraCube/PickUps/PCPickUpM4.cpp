// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUps/PCPickUpM4.h"

APCPickUpM4::APCPickUpM4()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4'"));
	if (GunMeshRef.Object)
	{
		GunMesh->SetSkeletalMesh(GunMeshRef.Object);
	}

	FName RowName = FName(*FString::FromInt(1));
	FString ContextString = TEXT("Item Data Context");

	if (ItemDataTable)
	{
		FInventoryItem* Row = ItemDataTable->FindRow<FInventoryItem>(RowName, ContextString);

		if (Row)
		{
			Item.ID = 1;
			Item.CurrentBullets = Row->Stats.MagSize;
			Item.TotalBullets = Row->Stats.MagSize * 3;
			Item.ItemType = EItemType::Primary;
		}
	}
}
