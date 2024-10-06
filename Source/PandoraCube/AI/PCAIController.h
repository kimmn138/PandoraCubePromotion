// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CharacterStat/CharacterStat.h"
#include "PCAIController.generated.h"

/**
 * 
 */
UCLASS()
class PANDORACUBE_API APCAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	APCAIController();

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY()
	FCharacterStats CharacterStats;
};
