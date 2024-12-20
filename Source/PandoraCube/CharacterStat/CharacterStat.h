#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStats : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStat")
    FName Name;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStat")
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStat")
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStat")
    float MaxSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStat")
    float MaxHp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStat")
    float AttackRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStat")
    float AttackRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStat")
    float AttackRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStat")
    TObjectPtr<class UBlackboardData> BBAsset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStat")
    TObjectPtr<class UBehaviorTree> BTAsset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStat")
    TObjectPtr<class USoundBase> ZombieSound;
};

USTRUCT(BlueprintType)
struct FPlayerStats : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat")
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat")
    float MaxSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat")
    float MaxHp;
};