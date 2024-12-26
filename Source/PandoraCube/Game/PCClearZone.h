// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCClearZone.generated.h"

UCLASS()
class PANDORACUBE_API APCClearZone : public AActor
{
	GENERATED_BODY()
	
public:
	APCClearZone();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> TriggerBox;
};
