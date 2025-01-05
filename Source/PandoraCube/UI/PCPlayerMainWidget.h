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

	FORCEINLINE void SetMaxAmmo(float NewMaxAmmo) { MaxAmmo = NewMaxAmmo; }
	void UpdateCurrentAmmoText(float NewCurrentAmmo);
	void UpdateMaxAmmoText(float NewMaxAmmo);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetAimImageVisibility(bool bIsVisible);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetDamageOverlayImageVisibility(bool bIsVisible);

protected:
	UPROPERTY()
	TObjectPtr<class UTextBlock> HpText;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HpInt;

	UPROPERTY()
	float MaxHp;

	UPROPERTY()
	TObjectPtr<class UTextBlock> CurrentAmmoTextInt;

	UPROPERTY()
	TObjectPtr<class UTextBlock> MaxAmmoTextInt;

	UPROPERTY()
	int32 MaxAmmo;

	UPROPERTY()
	int32 CurrentAmmo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> AimImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> DamageOverlayImage;
};
