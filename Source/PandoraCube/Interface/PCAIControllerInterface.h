// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "CharacterStat/CharacterStat.h"
#include "PCAIControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPCAIControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PANDORACUBE_API IPCAIControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UBlackboardData* GetBlackboardData() const = 0;
	virtual UBehaviorTree* GetBehaviorTree() const = 0;
	virtual FCharacterStats GetCharacterStats() const = 0;
};
