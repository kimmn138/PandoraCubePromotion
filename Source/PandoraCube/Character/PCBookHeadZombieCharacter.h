// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PCEnemyCharacterBase.h"
#include "PCBookHeadZombieCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PANDORACUBE_API APCBookHeadZombieCharacter : public APCEnemyCharacterBase
{
	GENERATED_BODY()
	
public:
	APCBookHeadZombieCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
};
