// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUps/PCPickUpShotgun.h"

APCPickUpShotgun::APCPickUpShotgun()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/MilitaryWeapSilver/Weapons/Shotgun_A.Shotgun_A'"));
	if (GunMeshRef.Object)
	{
		GunMesh->SetSkeletalMesh(GunMeshRef.Object);
	}

	FName RowName = FName(*FString::FromInt(4));
	FString ContextString = TEXT("Item Data Context");

	if (ItemDataTable)
	{
		FInventoryItem* Row = ItemDataTable->FindRow<FInventoryItem>(RowName, ContextString);

		if (Row)
		{
			Item.ID = 4;
			Item.CurrentBullets = Row->Stats.MagSize;
			Item.TotalBullets = Row->Stats.MagSize * 3;
			Item.ItemType = EItemType::Primary;
		}
	}
}
