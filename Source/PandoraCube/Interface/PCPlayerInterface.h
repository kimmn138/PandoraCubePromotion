// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PCPlayerInterface.generated.h"

USTRUCT(BlueprintType)
struct FHandSwayValues
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	float SideMov = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	float MouseX = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	float MouseY = 0.0f;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPCPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PANDORACUBE_API IPCPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PCPlayerInterface")
	FTransform GetLeftHandSocketTransform() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PCPlayerInterface")
	FHandSwayValues GetHandSwayFloats() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PCPlayerInterface")
	bool GetIsAim() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PCPlayerInterface")
	bool GetStopLeftHandIK() const;
};
