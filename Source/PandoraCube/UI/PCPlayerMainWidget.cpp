// Fill out your copyright notice in the Description page of Project Settings.


#include "PCPlayerMainWidget.h"
#include "UI/PCPlayerMainWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

UPCPlayerMainWidget::UPCPlayerMainWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
	MaxAmmo = 0;
	CurrentAmmo = 0;
}

void UPCPlayerMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpInt = Cast<UTextBlock>(GetWidgetFromName(TEXT("HpInt")));
	ensure(HpInt);

	HpText = Cast<UTextBlock>(GetWidgetFromName(TEXT("HpText")));
	ensure(HpText);

	CurrentAmmoTextInt = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentAmmo")));
	ensure(CurrentAmmoTextInt);

	MaxAmmoTextInt = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxAmmo")));
	ensure(MaxAmmoTextInt);

	GoalText = Cast<UTextBlock>(GetWidgetFromName(TEXT("GoalText")));
	ensure(GoalText);

	WeaponText = Cast<UTextBlock>(GetWidgetFromName(TEXT("WeaponText")));
	ensure(WeaponText);
}

void UPCPlayerMainWidget::UpdateHpText(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if (HpInt)
	{
		int32 CurrentHpInt = static_cast<int32>(NewCurrentHp);
		FText HpTextValue = FText::AsNumber(CurrentHpInt);

		HpInt->SetText(HpTextValue);

		if (CurrentHpInt <= 30)
		{
			HpInt->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f)));
			HpText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f)));
		}
		else if (CurrentHpInt <= 70)
		{
			HpInt->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.5f, 0.0f, 1.0f)));
			HpText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.5f, 0.0f, 1.0f)));
		}
		else
		{
			HpInt->SetColorAndOpacity(FSlateColor(FLinearColor(0.0f, 1.0f, 0.0f, 1.0f)));
			HpText->SetColorAndOpacity(FSlateColor(FLinearColor(0.0f, 1.0f, 0.0f, 1.0f)));
		}
	}
}

void UPCPlayerMainWidget::UpdateCurrentAmmoText(float NewCurrentAmmo)
{
	ensure(CurrentAmmo >= 0.0f);
	if (CurrentAmmoTextInt)
	{
		int32 CurrentAmmoInt = static_cast<int32>(NewCurrentAmmo);
		FText CurrentAmmoTextValue = FText::AsNumber(CurrentAmmoInt);

		CurrentAmmoTextInt->SetText(CurrentAmmoTextValue);
	}
}

void UPCPlayerMainWidget::UpdateMaxAmmoText(float NewMaxAmmo)
{
	ensure(MaxAmmo >= 0.0f);
	if (MaxAmmoTextInt)
	{
		int32 MaxAmmoInt = static_cast<int32>(NewMaxAmmo);
		FText MaxAmmoTextValue = FText::AsNumber(MaxAmmoInt);

		MaxAmmoTextInt->SetText(MaxAmmoTextValue);
	}
}

void UPCPlayerMainWidget::UpdateWeaponText(FName NewWeaponName)
{
	if (WeaponText)
	{
		WeaponText->SetText(FText::FromString(NewWeaponName.ToString()));
	}
}

void UPCPlayerMainWidget::SetAimImageVisibility(bool bIsVisible)
{
	if (AimImage)
	{
		AimImage->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UPCPlayerMainWidget::SetDamageOverlayImageVisibility(bool bIsVisible)
{
	if (DamageOverlayImage)
	{
		DamageOverlayImage->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UPCPlayerMainWidget::SetGoalTextHidden()
{
	if (GoalText)
	{
		GoalText->SetVisibility(ESlateVisibility::Hidden);
	}
}
