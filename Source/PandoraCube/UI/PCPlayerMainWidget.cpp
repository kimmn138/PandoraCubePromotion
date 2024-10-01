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

	HpText = Cast<UTextBlock>(GetWidgetFromName(TEXT("HpInt")));
	ensure(HpText);

}

void UPCPlayerMainWidget::UpdateHpText(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if (HpText)
	{
		int32 CurrentHpInt = static_cast<int32>(NewCurrentHp);
		FText HpTextValue = FText::AsNumber(CurrentHpInt);

		HpText->SetText(HpTextValue);
	}
}
