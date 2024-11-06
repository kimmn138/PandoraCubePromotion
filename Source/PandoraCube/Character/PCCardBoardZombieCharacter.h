// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PCEnemyCharacterBase.h"
#include "PCCardBoardZombieCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PANDORACUBE_API APCCardBoardZombieCharacter : public APCEnemyCharacterBase
{
	GENERATED_BODY()
	
public:
	APCCardBoardZombieCharacter();

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void SetFleeing();
};
