// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUps/PCPickUpBase.h"
#include "PCPickUpPistol.generated.h"

/**
 * 
 */
UCLASS()
class PANDORACUBE_API APCPickUpPistol : public APCPickUpBase
{
	GENERATED_BODY()
	
public:
	APCPickUpPistol();
	
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void ApplyOverlayMaterial(USkeletalMeshComponent* SkeletalMeshComp, UMaterialInterface* OverlayMaterial);
};
