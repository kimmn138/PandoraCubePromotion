// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PCBookHeadZombieCharacter.h"
#include "CharacterStat/PCCharacterStatComponent.h"
#include "Animation/AnimMontage.h"
#include "PCComboActionData.h"

APCBookHeadZombieCharacter::APCBookHeadZombieCharacter()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/BookHeadMonster/Meshes/BookHeadMonster.BookHeadMonster'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/PandoraCube/Blueprints/Animation/ABP_BookHeadMonsterAnimationBlueprint.ABP_BookHeadMonsterAnimationBlueprint_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/PandoraCube/Blueprints/Animation/AM_BookHeadZombieComboAttack.AM_BookHeadZombieComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPCComboActionData> ComboActionDataRef(TEXT("/Script/PandoraCube.PCComboActionData'/Game/PandoraCube/CharacterAction/PCA_ComboAttack.PCA_ComboAttack'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/PandoraCube/Blueprints/Animation/AM_BookHeadZombieDead.AM_BookHeadZombieDead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/PandoraCube/Blueprints/Animation/AM_BookHeadZombieHit.AM_BookHeadZombieHit'"));
	if (HitMontageRef.Object)
	{
		HitMontage = HitMontageRef.Object;
	}

	FString ContextString = TEXT("Stat Data Context");
	if (StatDataTable)
	{
		FCharacterStats* Row = StatDataTable->FindRow<FCharacterStats>(TEXT("2"), ContextString);

		if (Row)
		{
			CurrentStats.Damage = Row->Damage;
			CurrentStats.MaxHp = Row->MaxHp;
			CurrentStats.Speed = Row->Speed;
			CurrentStats.MaxSpeed = Row->MaxSpeed;
			CurrentStats.AttackRate = Row->AttackRate;
			CurrentStats.AttackRange = Row->AttackRange;
			CurrentStats.AttackRadius = Row->AttackRadius;
			CurrentStats.BBAsset = Row->BBAsset;
			CurrentStats.BTAsset = Row->BTAsset;
			Stat->SetMaxHp(Row->MaxHp);
			Stat->SetCurrentHp(Row->MaxHp);
		}
	}
}
