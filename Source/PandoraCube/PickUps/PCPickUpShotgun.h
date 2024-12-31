// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUps/PCPickUpBase.h"
#include "PCPickUpShotgun.generated.h"

/**
 * 
 */
UCLASS()
class PANDORACUBE_API APCPickUpShotgun : public APCPickUpBase
{
	GENERATED_BODY()
	
public:
	APCPickUpShotgun();

	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void ApplyOverlayMaterial(USkeletalMeshComponent* SkeletalMeshComp, UMaterialInterface* OverlayMaterial);
};
