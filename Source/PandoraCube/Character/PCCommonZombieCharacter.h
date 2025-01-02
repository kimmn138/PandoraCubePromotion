// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PCEnemyCharacterBase.h"
#include "PCCommonZombieCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PANDORACUBE_API APCCommonZombieCharacter : public APCEnemyCharacterBase
{
	GENERATED_BODY()
	
public:
	APCCommonZombieCharacter();

protected:
	virtual void BeginPlay() override;
};
