// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PCInventoryComponent.h"

// Sets default values for this component's properties
UPCInventoryComponent::UPCInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	FDynamicInventoryItem NewItem;
	NewItem.ID = 1;
	NewItem.Bullets = 30;
	NewItem.Scope = 1;

	Inventory.Add(NewItem);

	NewItem.ID = 2;
	NewItem.Bullets = 25;
	NewItem.Scope = 0;

	Inventory.Add(NewItem);

}

// Called when the game starts
void UPCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPCInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

