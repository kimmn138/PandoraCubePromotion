// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUps/PCPickUpAK.h"
#include "Materials/MaterialInstanceDynamic.h"

APCPickUpAK::APCPickUpAK()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ka47/SK_KA47.SK_KA47'"));
	if (GunMeshRef.Object)
	{
		GunMesh->SetSkeletalMesh(GunMeshRef.Object);
	}
}

void APCPickUpAK::BeginPlay()
{
	Super::BeginPlay();

	if (GunMesh)
	{
		UMaterialInterface* OverlayMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/PandoraCube/Materials/Outline_Mat.Outline_Mat"));
		ApplyOverlayMaterial(GunMesh, OverlayMaterial);
	}

	FName RowName = FName(*FString::FromInt(2));
	FString ContextString = TEXT("Item Data Context");

	if (ItemDataTable)
	{
		const FInventoryItem* Row = ItemDataTable->FindRow<FInventoryItem>(RowName, ContextString);

		if (Row)
		{
			Item.ID = 2;
			Item.CurrentBullets = Row->Stats.MagSize;
			Item.TotalBullets = Row->Stats.MagSize * 3;
			Item.ItemType = EItemType::Primary;
		}
	}

}

void APCPickUpAK::ApplyOverlayMaterial(USkeletalMeshComponent* SkeletalMeshComp, UMaterialInterface* OverlayMaterial)
{
	if (!SkeletalMeshComp || !OverlayMaterial)
	{
		return;
	}

	SkeletalMeshComp->OverlayMaterial = OverlayMaterial;
	SkeletalMeshComp->MarkRenderStateDirty();
}
