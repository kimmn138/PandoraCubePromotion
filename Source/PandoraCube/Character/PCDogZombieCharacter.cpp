// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PCDogZombieCharacter.h"
#include "CharacterStat/PCCharacterStatComponent.h"
#include "Animation/AnimMontage.h"
#include "PCComboActionData.h"
#include "Game/PCGameInstance.h"
#include "PCPlayerCharacter.h"
#include "Engine/DamageEvents.h"
#include "Engine/AssetManager.h"
#include "Components/CapsuleComponent.h"

APCDogZombieCharacter::APCDogZombieCharacter()
{
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/PandoraCube/Blueprints/Animation/ABP_DogZombieAnimationBlueprint.ABP_DogZombieAnimationBlueprint_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/PandoraCube/Blueprints/Animation/AM_DogZombieComboAttack.AM_DogZombieComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPCComboActionData> ComboActionDataRef(TEXT("/Script/PandoraCube.PCComboActionData'/Game/PandoraCube/CharacterAction/PCA_ComboAttack.PCA_ComboAttack'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/PandoraCube/Blueprints/Animation/AM_DogZombieDead.AM_DogZombieDead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/PandoraCube/Blueprints/Animation/AM_DogZombieHit.AM_DogZombieHit'"));
	if (HitMontageRef.Object)
	{
		HitMontage = HitMontageRef.Object;
	}

	FString ContextString = TEXT("Stat Data Context");
	if (StatDataTable)
	{
		FCharacterStats* Row = StatDataTable->FindRow<FCharacterStats>(TEXT("3"), ContextString);

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

	bHasDealtDamage = false;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APCDogZombieCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APCDogZombieCharacter::OnOverlapEnd);

	GetMesh()->SetRelativeLocationAndRotation(FVector(-33.718998, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
}

void APCDogZombieCharacter::BeginPlay()
{
	Super::BeginPlay();

	FString ContextString = TEXT("Stat Data Context");
	if (StatDataTable)
	{
		FCharacterStats* Row = StatDataTable->FindRow<FCharacterStats>(TEXT("3"), ContextString);

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

void APCDogZombieCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FSoftObjectPath MeshPath(TEXT("/Game/DogZombie/Meshes/SK_DogZombie.SK_DogZombie"));

	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(MeshPath, FStreamableDelegate::CreateUObject(this, &APCEnemyCharacterBase::NPCMeshLoadCompleted));
}

void APCDogZombieCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bHasDealtDamage) return;

	if (OtherActor && OtherActor->IsA(APCPlayerCharacter::StaticClass()))
	{
		FDamageEvent DamageEvent;
		OtherActor->TakeDamage(CurrentStats.Damage, DamageEvent, GetController(), this);

		bHasDealtDamage = true;
	}
}

void APCDogZombieCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA(APCPlayerCharacter::StaticClass()))
	{
		bHasDealtDamage = false;
	}
}
