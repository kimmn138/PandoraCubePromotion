// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "PCBulletHole.generated.h"

UCLASS()
class PANDORACUBE_API APCBulletHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APCBulletHole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Decal)
	TObjectPtr<class UDecalComponent> BulletHole;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Decal)
	TObjectPtr<class UMaterialInstanceDynamic> DynamicMaterialInstance;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timeline, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTimelineComponent> BulletHoleTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timeline, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCurveFloat> BulletHoleCurve;

	FOnTimelineFloat BulletHoleInterpFunction{};

	UFUNCTION()
	void BulletHoleTimelineProgress(float Value);
};
