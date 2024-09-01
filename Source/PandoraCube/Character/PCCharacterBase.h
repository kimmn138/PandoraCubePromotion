// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon/PCWeaponBase.h"
#include "Interface/PCPlayerInterface.h"
#include "PCCharacterBase.generated.h"

UCLASS(BlueprintType, Blueprintable)
class PANDORACUBE_API APCCharacterBase : public ACharacter, public IPCPlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APCCharacterBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual FTransform GetLeftHandSocketTransform_Implementation() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class APCWeaponBase> WeaponClass;

	UPROPERTY()
	APCWeaponBase* EquippedWeapon;
};
