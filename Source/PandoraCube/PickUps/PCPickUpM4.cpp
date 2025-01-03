// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUps/PCPickUpM4.h"
#include "Materials/MaterialInstanceDynamic.h"

APCPickUpM4::APCPickUpM4()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4'"));
	if (GunMeshRef.Object)
	{
		GunMesh->SetSkeletalMesh(GunMeshRef.Object);
	}
}

void APCPickUpM4::BeginPlay()
{
	Super::BeginPlay();

	if (GunMesh)
	{
		UMaterialInterface* OverlayMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/PandoraCube/Materials/Outline_Mat.Outline_Mat"));
		ApplyOverlayMaterial(GunMesh, OverlayMaterial);
	}

	FName RowName = FName(*FString::FromInt(1));
	FString ContextString = TEXT("Item Data Context");

	if (ItemDataTable)
	{
		const FInventoryItem* Row = ItemDataTable->FindRow<FInventoryItem>(RowName, ContextString);

		if (Row)
		{
			Item.ID = 1;
			Item.CurrentBullets = Row->Stats.MagSize;
			Item.TotalBullets = Row->Stats.MagSize * 5;
			Item.ItemType = EItemType::Primary;
		}
	}
}

void APCPickUpM4::ApplyOverlayMaterial(USkeletalMeshComponent* SkeletalMeshComp, UMaterialInterface* OverlayMaterial)
{
	if (!SkeletalMeshComp || !OverlayMaterial)
	{
		return;
	}

	SkeletalMeshComp->OverlayMaterial = OverlayMaterial;
	SkeletalMeshComp->MarkRenderStateDirty();
}
