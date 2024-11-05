// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/ItemTypes.h"
#include "PCInventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentAmmoChangedDelegate, float /*CurrentAmmo*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxAmmoChangedDelegate, float /*CurrentAmmo*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PANDORACUBE_API UPCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPCInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	FOnCurrentAmmoChangedDelegate OnCurrentAmmoChanged;
	FOnMaxAmmoChangedDelegate OnMaxAmmoChanged;

	FORCEINLINE int32 GetMaxAmmo() { return MaxAmmo; }
	FORCEINLINE int32 GetCurrentAmmo() { return CurrentAmmo; }
	FORCEINLINE void SetMaxAmmo(int32 NewAmmo) { MaxAmmo = NewAmmo; OnMaxAmmoChanged.Broadcast(MaxAmmo); }
	FORCEINLINE void SetCurrentAmmo(int32 NewAmmo) { CurrentAmmo = NewAmmo; OnCurrentAmmoChanged.Broadcast(CurrentAmmo);
	}
	int32 ApplyAmmo(int32 InAmmo);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FDynamicInventoryItem> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxItemCount = 2;

protected:
	void SetAmmo(int32 NewAmmo);

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	int32 MaxAmmo;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	int32 CurrentAmmo;
};
