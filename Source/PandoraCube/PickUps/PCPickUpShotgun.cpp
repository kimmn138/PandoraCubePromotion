// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUps/PCPickUpShotgun.h"
#include "Materials/MaterialInstanceDynamic.h"

APCPickUpShotgun::APCPickUpShotgun()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/MilitaryWeapSilver/Weapons/Shotgun_A.Shotgun_A'"));
	if (GunMeshRef.Object)
	{
		GunMesh->SetSkeletalMesh(GunMeshRef.Object);
	}
}

void APCPickUpShotgun::BeginPlay()
{
	Super::BeginPlay();

	if (GunMesh)
	{
		UMaterialInterface* OverlayMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/PandoraCube/Materials/Outline_Mat.Outline_Mat"));
		ApplyOverlayMaterial(GunMesh, OverlayMaterial);
	}

	FName RowName = FName(*FString::FromInt(4));
	FString ContextString = TEXT("Item Data Context");

	if (ItemDataTable)
	{
		const FInventoryItem* Row = ItemDataTable->FindRow<FInventoryItem>(RowName, ContextString);

		if (Row)
		{
			Item.ID = 4;
			Item.CurrentBullets = Row->Stats.MagSize;
			Item.TotalBullets = Row->Stats.MagSize * 5;
			Item.ItemType = EItemType::Primary;
		}
	}
}

void APCPickUpShotgun::ApplyOverlayMaterial(USkeletalMeshComponent* SkeletalMeshComp, UMaterialInterface* OverlayMaterial)
{
	if (!SkeletalMeshComp || !OverlayMaterial)
	{
		return;
	}

	SkeletalMeshComp->OverlayMaterial = OverlayMaterial;
	SkeletalMeshComp->MarkRenderStateDirty();
}
