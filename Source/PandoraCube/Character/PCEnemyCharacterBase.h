// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/PCAnimationAttackInterface.h"
#include "Engine/DataTable.h"
#include "CharacterStat/CharacterStat.h"
#include "Interface/PCAIControllerInterface.h"
#include "Interface/PCCharacterInterface.h"
#include "Engine/StreamableManager.h"
#include "PCEnemyCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnZombieDeath);

UCLASS()
class PANDORACUBE_API APCEnemyCharacterBase : public ACharacter, public IPCAnimationAttackInterface, public IPCAIControllerInterface, public IPCCharacterInterface
{
	GENERATED_BODY()

public:
	APCEnemyCharacterBase();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginDestroy() override;

	virtual void PostInitializeComponents() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	void TakeKnockBack(const FVector& HitLocation, const FVector& ImpactDirection, float Force);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
	bool bImmediateChase;

	bool bIsDead = false;

	void SetImmediateChase(bool bChase);

	UPROPERTY(BlueprintAssignable, Category = "Zombie")
	FOnZombieDeath OnZombieDeath;

	UPROPERTY()
	FTimerHandle DeadTimerHandle;

protected:
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

	UPROPERTY()
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;
	bool bIsAIActive = false;

	UFUNCTION()
	void PauseAnimation();

	UFUNCTION()
	void ResumeAnimation();

protected:
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> HitMontage;

	void PlayDeadAnimation();
	void PlayHitAnimation();
	virtual void SetDead();

	float DeadEventDelayTime = 4.0f;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPCCharacterStatComponent> Stat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> StatDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FCharacterStats CurrentStats;

	UPROPERTY()
	TObjectPtr<class UAudioComponent> AudioComponent;

public:
	virtual UBlackboardData* GetBlackboardData() const override { return CurrentStats.BBAsset; }
	virtual UBehaviorTree* GetBehaviorTree() const override { return CurrentStats.BTAsset; }
	virtual FCharacterStats GetCharacterStats() const override { return CurrentStats; }

	void NPCMeshLoadCompleted();

	bool bIsFleeing = false;

	void CheckPlayerProximity();

protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;
	virtual bool GetAIIsFleeing() override;
	virtual void SetAIIsFleeing(bool NewFleeing) override;
	virtual USoundBase* GetAISound() override;
	virtual FVector GetAILocation() override;
	virtual UAudioComponent* GetAIAudioComponent() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	FAICharacterAttackFinished OnAttackFinished;

	void NotifyComboActionEnd();

	TSharedPtr<FStreamableHandle> NPCMeshHandle;

protected:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TArray<TSubclassOf<class APCPickUpBase>> WeaponClasses;

	void SpawnRandomWeapon();

	UPROPERTY(EditAnywhere, Category = "Loot")
	float DropChance = 0.05f;
};
