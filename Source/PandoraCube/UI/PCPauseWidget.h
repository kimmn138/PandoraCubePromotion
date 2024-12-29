// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PCPauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class PANDORACUBE_API UPCPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void ResumeGame();
};
