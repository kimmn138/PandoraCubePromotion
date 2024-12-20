// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PCCommonZombieCharacter.h"
#include "CharacterStat/PCCharacterStatComponent.h"
#include "Animation/AnimMontage.h"
#include "PCComboActionData.h"

APCCommonZombieCharacter::APCCommonZombieCharacter()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Zombie/Mesh/SK_Zombie.SK_Zombie'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/PandoraCube/Blueprints/Animation/ABP_CommonZombieAnimationBlueprint.ABP_CommonZombieAnimationBlueprint_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/PandoraCube/Blueprints/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPCComboActionData> ComboActionDataRef(TEXT("/Script/PandoraCube.PCComboActionData'/Game/PandoraCube/CharacterAction/PCA_ComboAttack.PCA_ComboAttack'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/PandoraCube/Blueprints/Animation/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/PandoraCube/Blueprints/Animation/AM_Hit.AM_Hit'"));
	if (HitMontageRef.Object)
	{
		HitMontage = HitMontageRef.Object;
	}

	FString ContextString = TEXT("Stat Data Context");
	if (StatDataTable)
	{
		FCharacterStats* Row = StatDataTable->FindRow<FCharacterStats>(TEXT("1"), ContextString);

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
			CurrentStats.ZombieSound = Row->ZombieSound;
			Stat->SetMaxHp(Row->MaxHp);
			Stat->SetCurrentHp(Row->MaxHp);
		}
	}
}
