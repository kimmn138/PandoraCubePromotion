#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Weapon/PCWeaponBase.h"
#include "ItemTypes.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    IT_Rifle   UMETA(DisplayName = "Rifle"),
    IT_Pistol UMETA(DisplayName = "Pistol")
};

USTRUCT(BlueprintType)
struct FWeaponStats : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStat")
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStat")
    float FireRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStat")
    int32 MagSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStat")
    int32 Range;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStat")
    float ProceduralRecoil;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStat")
    float InputRecoil;
};

USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* Icon;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    TSubclassOf<APCWeaponBase> WeaponClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FWeaponStats Stats;
};
