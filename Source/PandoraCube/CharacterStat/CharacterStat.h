#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStats : public FTableRowBase
{
    GENERATED_BODY()
    
    FCharacterStats()
        : Damage(0.0f)
        , Speed(0.0f)
        , MaxSpeed(0.0f)
        , MaxHp(100.0f)
        , AttackRate(1.0f)
        , AttackRange(100.0f)
        , AttackRadius(50.0f)
    {
    }

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
    
    FPlayerStats()
        : Speed(300.0f)
        , MaxSpeed(600.0f)
        , MaxHp(100.0f)
    {
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat")
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat")
    float MaxSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat")
    float MaxHp;
};