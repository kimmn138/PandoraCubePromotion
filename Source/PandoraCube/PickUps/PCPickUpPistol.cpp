// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUps/PCPickUpPistol.h"

APCPickUpPistol::APCPickUpPistol()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/MilitaryWeapSilver/Weapons/Pistols_A.Pistols_A'"));
	if (GunMeshRef.Object)
	{
		GunMesh->SetSkeletalMesh(GunMeshRef.Object);
	}

	FName RowName = FName(*FString::FromInt(3));
	FString ContextString = TEXT("Item Data Context");

	if (ItemDataTable)
	{
		FInventoryItem* Row = ItemDataTable->FindRow<FInventoryItem>(RowName, ContextString);

		if (Row)
		{
			Item.ID = 3;
			Item.CurrentBullets = Row->Stats.MagSize;
			Item.TotalBullets = Row->Stats.MagSize * 3;
			Item.ItemType = EItemType::Secondary;
		}
	}
}
