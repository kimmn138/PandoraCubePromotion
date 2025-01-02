// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PCEnemyCharacterBase.h"
#include "PCDogZombieCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PANDORACUBE_API APCDogZombieCharacter : public APCEnemyCharacterBase
{
	GENERATED_BODY()
	
public:
	APCDogZombieCharacter();

protected:
	virtual void BeginPlay() override;
};
