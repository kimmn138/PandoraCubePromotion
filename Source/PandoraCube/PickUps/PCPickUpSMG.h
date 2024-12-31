// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUps/PCPickUpBase.h"
#include "PCPickUpSMG.generated.h"

/**
 * 
 */
UCLASS()
class PANDORACUBE_API APCPickUpSMG : public APCPickUpBase
{
	GENERATED_BODY()
	
public:
	APCPickUpSMG();

	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void ApplyOverlayMaterial(USkeletalMeshComponent* SkeletalMeshComp, UMaterialInterface* OverlayMaterial);

};
