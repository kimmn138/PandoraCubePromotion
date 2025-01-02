// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PCCardBoardZombieCharacter.h"
#include "CharacterStat/PCCharacterStatComponent.h"
#include "Animation/AnimMontage.h"
#include "PCComboActionData.h"
#include "AI/PCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/PCAI.h"
#include "Game/PCGameInstance.h"

APCCardBoardZombieCharacter::APCCardBoardZombieCharacter()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/PandoraCube/Blueprints/Animation/ABP_CardBoardZombieAnimationBlueprint.ABP_CardBoardZombieAnimationBlueprint_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/PandoraCube/Blueprints/Animation/AM_CardBoardZombieComboAttack.AM_CardBoardZombieComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPCComboActionData> ComboActionDataRef(TEXT("/Script/PandoraCube.PCComboActionData'/Game/PandoraCube/CharacterAction/PCA_ComboAttack.PCA_ComboAttack'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/PandoraCube/Blueprints/Animation/AM_CardBoardZombieDead.AM_CardBoardZombieDead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/PandoraCube/Blueprints/Animation/AM_CardBoardZombieHit.AM_CardBoardZombieHit'"));
	if (HitMontageRef.Object)
	{
		HitMontage = HitMontageRef.Object;
	}

	FString ContextString = TEXT("Stat Data Context");
	if (StatDataTable)
	{
		FCharacterStats* Row = StatDataTable->FindRow<FCharacterStats>(TEXT("4"), ContextString);

		if (Row)
		{
			CurrentStats.Speed = Row->Speed;
			CurrentStats.MaxSpeed = Row->MaxSpeed;
			CurrentStats.AttackRate = Row->AttackRate;
			CurrentStats.AttackRange = Row->AttackRange;
			CurrentStats.AttackRadius = Row->AttackRadius;
			CurrentStats.BBAsset = Row->BBAsset;
			CurrentStats.BTAsset = Row->BTAsset;
			CurrentStats.ZombieSound = Row->ZombieSound;
		}
	}
}

void APCCardBoardZombieCharacter::BeginPlay()
{
	Super::BeginPlay();

	FString ContextString = TEXT("Stat Data Context");
	if (StatDataTable)
	{
		FCharacterStats* Row = StatDataTable->FindRow<FCharacterStats>(TEXT("4"), ContextString);

		if (Row)
		{
			float DifficultyMultiplier = 1.0f;
			if (GetWorld())
			{
				UPCGameInstance* GameInstance = Cast<UPCGameInstance>(GetWorld()->GetGameInstance());
				if (GameInstance)
				{
					DifficultyMultiplier = GameInstance->GetDifficultyMultiplier(GameInstance->GetDifficulty());
				}
			}

			CurrentStats.Damage = Row->Damage * DifficultyMultiplier;
			CurrentStats.MaxHp = Row->MaxHp * DifficultyMultiplier;

			Stat->SetMaxHp(CurrentStats.MaxHp);
			Stat->SetCurrentHp(CurrentStats.MaxHp);
		}
	}
}

float APCCardBoardZombieCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!bIsFleeing)
	{
		SetFleeing();
	}

	return 0.0f;
}

void APCCardBoardZombieCharacter::SetFleeing()
{
	bIsFleeing = true;
	UE_LOG(LogTemp, Warning, TEXT("SSSSIBAL?"));
	UBlackboardComponent* BlackboardComp = Cast<APCAIController>(GetController())->GetBlackboardComponent();
	if (BlackboardComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("SSSSIBAL!"));
		BlackboardComp->SetValueAsBool(BBKEY_FLEEING, bIsFleeing);
	}
}
