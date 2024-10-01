// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PCPlayerMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class PANDORACUBE_API UPCPlayerMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPCPlayerMainWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateHpText(float NewCurrentHp);

protected:
	UPROPERTY()
	TObjectPtr<class UTextBlock> HpText;

	UPROPERTY()
	float MaxHp;
};
