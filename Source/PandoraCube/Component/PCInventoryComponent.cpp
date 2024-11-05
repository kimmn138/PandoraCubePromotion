// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PCInventoryComponent.h"

// Sets default values for this component's properties
UPCInventoryComponent::UPCInventoryComponent()
{
	Inventory.SetNum(2);

	MaxAmmo = 0;
	CurrentAmmo = MaxAmmo;
}

// Called when the game starts
void UPCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	SetAmmo(MaxAmmo);
	
}

int32 UPCInventoryComponent::ApplyAmmo(int32 InAmmo)
{
	const int32 PrevAmmo = CurrentAmmo;
	const int32 ActualAmmo = FMath::Clamp<int32>(InAmmo, 0, InAmmo);

	SetAmmo(PrevAmmo - ActualAmmo);

	return ActualAmmo;
}

void UPCInventoryComponent::SetAmmo(int32 NewAmmo)
{
	CurrentAmmo = FMath::Clamp<int32>(NewAmmo, 0.0f, MaxAmmo);

	OnCurrentAmmoChanged.Broadcast(CurrentAmmo);
}

