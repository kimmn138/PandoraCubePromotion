#pragma once

#include "CoreMinimal.h"
#include "AnimState.generated.h"

UENUM(BlueprintType)
enum class EAnimState : uint8
{
    Hands  UMETA(DisplayName = "Hands"),
    Rifle  UMETA(DisplayName = "Rifle"),
    Pistol  UMETA(DisplayName = "Pistol"),
    Melee  UMETA(DisplayName = "Melee")
};