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
    TSoftClassPtr<APCWeaponBase> WeaponClass;
};