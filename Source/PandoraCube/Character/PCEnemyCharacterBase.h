// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/PCAnimationAttackInterface.h"
#include "Engine/DataTable.h"
#include "CharacterStat/CharacterStat.h"
#include "Interface/PCAIControllerInterface.h"
#include "Interface/PCCharacterInterface.h"
#include "PCEnemyCharacterBase.generated.h"

UCLASS()
class PANDORACUBE_API APCEnemyCharacterBase : public ACharacter, public IPCAnimationAttackInterface, public IPCAIControllerInterface, public IPCCharacterInterface
{
	GENERATED_BODY()

public:
	APCEnemyCharacterBase();

	virtual void PostInitializeComponents() override;

	void TakeKnockBack(const FVector& HitLocation, const FVector& ImpactDirection, float Force);

protected:
	void Attack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPCComboActionData> ComboActionData;

	void ProcessComboCommand();

	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void SetComboCheckTimer();
	void ComboCheck();

	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

protected:
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

	float DeadEventDelayTime = 5.0f;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPCCharacterStatComponent> Stat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> StatDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FCharacterStats CurrentStats;

public:
	virtual UBlackboardData* GetBlackboardData() const override { return CurrentStats.BBAsset; }
	virtual UBehaviorTree* GetBehaviorTree() const override { return CurrentStats.BTAsset; }
	virtual FCharacterStats GetCharacterStats() const override { return CurrentStats; }

protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	FAICharacterAttackFinished OnAttackFinished;

	void NotifyComboActionEnd();
};
