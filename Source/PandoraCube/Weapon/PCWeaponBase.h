// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCWeaponBase.generated.h"

UCLASS()
class PANDORACUBE_API APCWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	APCWeaponBase();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Aim)
	TObjectPtr<class USceneComponent> AimOffset;
};
