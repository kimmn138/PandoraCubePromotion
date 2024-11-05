#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Weapon/PCWeaponBase.h"
#include "Animation/AnimState.h"
#include "ItemTypes.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    IT_Rifle  UMETA(DisplayName = "Rifle"),
    IT_Pistol UMETA(DisplayName = "Pistol"),
    IT_Shotgun UMETA(DisplayName = "Shotgun"),
    IT_Hand UMETA(DisplayName = "Hand"),
    IT_SMG UMETA(DisplayName = "SMG")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Primary  UMETA(DisplayName = "Primary"),
    Secondary UMETA(DisplayName = "Secondary")
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStat")
    float ReloadTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStat")
    float Force;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStat")
    int32 ShoutgunPelletCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStat")
    float ShoutgunSpreadAngle;
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
    EWeaponType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemType ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    TSubclassOf<APCWeaponBase> WeaponClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FWeaponStats Stats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    TObjectPtr<class UAnimMontage> ReloadAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    TSubclassOf<class APCPickUpBase> PickupClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EAnimState AnimState;
};

USTRUCT(BlueprintType)
struct FDynamicInventoryItem : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DynamicItem")
    int32 ID = -1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DynamicItem")
    int32 CurrentBullets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DynamicItem")
    int32 TotalBullets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DynamicItem")
    int32 Scope;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DynamicItem")
    EItemType ItemType;
};
