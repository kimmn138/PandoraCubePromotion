// Fill out your copyright notice in the Description page of Project Settings.


#include "PCPlayerMainWidget.h"
#include "UI/PCPlayerMainWidget.h"
#include "Components/TextBlock.h"

UPCPlayerMainWidget::UPCPlayerMainWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UPCPlayerMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpInt = Cast<UTextBlock>(GetWidgetFromName(TEXT("HpInt")));
	ensure(HpInt);

	HpText = Cast<UTextBlock>(GetWidgetFromName(TEXT("HpText")));
	ensure(HpText);

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