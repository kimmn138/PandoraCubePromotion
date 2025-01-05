// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCMotionBlurManager.generated.h"

UCLASS()
class PANDORACUBE_API APCMotionBlurManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APCMotionBlurManager();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetMotionBlurEnabled(bool bEnable);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	bool IsMotionBlurEnabled() const { return bIsMotionBlurEnabled; }

private:
	bool bIsMotionBlurEnabled;

};
