// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PCCommonZombieCharacter.h"
#include "CharacterStat/PCCharacterStatComponent.h"

APCCommonZombieCharacter::APCCommonZombieCharacter()
{
	FString ContextString = TEXT("Stat Data Context");
	if (StatDataTable)
	{
		FCharacterStats* Row = StatDataTable->FindRow<FCharacterStats>(TEXT("1"), ContextString);

		if (Row)
		{
			CurrentStats.Damage = Row->Damage;
			CurrentStats.MaxHp = Row->MaxHp;
			CurrentStats.AttackRate = Row->AttackRate;
			CurrentStats.AttackRange = Row->AttackRange;
			CurrentStats.AttackRadius = Row->AttackRadius;
			Stat->SetMaxHp(Row->MaxHp);
			Stat->SetCurrentHp(Row->MaxHp);
		}
	}
}
