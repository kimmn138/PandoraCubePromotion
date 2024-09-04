// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCBlood.generated.h"

UCLASS()
class PANDORACUBE_API APCBlood : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APCBlood();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UDecalComponent> Blood;
};
