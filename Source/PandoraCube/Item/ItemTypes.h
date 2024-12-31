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
    
    FWeaponStats()
        : Damage(10.0f)
        , FireRate(0.5f)
        , MagSize(30)
        , Range(1000)
        , ProceduralRecoil(0.1f)
        , InputRecoil(0.2f)
        , ReloadTime(2.0f)
        , Force(100.0f)
        , ShoutgunPelletCount(8)
        , ShoutgunSpreadAngle(15.0f)
    {
    }

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
    
    FInventoryItem()
        : Name(NAME_None)
        , Icon(nullptr)
        , Type(EWeaponType::IT_Hand)
        , ItemType(EItemType::Primary)
        , WeaponClass(nullptr)
        , Stats()
        , ReloadAnimation(nullptr)
        , PickupClass(nullptr)
        , AnimState(EAnimState::Hands)
    {
    }

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
    
    FDynamicInventoryItem()
        : ID(-1)
        , CurrentBullets(0)
        , TotalBullets(0)
        , Scope(0)
        , ItemType(EItemType::Primary)
    {
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DynamicItem")
    int32 ID = -1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DynamicItem")
    int32 CurrentBullets = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DynamicItem")
    int32 TotalBullets = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DynamicItem")
    int32 Scope;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DynamicItem")
    EItemType ItemType;
};
