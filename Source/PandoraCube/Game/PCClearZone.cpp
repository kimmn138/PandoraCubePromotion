// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCClearZone.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/PCPlayerCharacter.h"
#include "Game/PCGameMode.h"
#include "Game/PCGameInstance.h"

APCClearZone::APCClearZone()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APCClearZone::OnOverlapBegin);
}

void APCClearZone::BeginPlay()
{
	Super::BeginPlay();
	
}

void APCClearZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APCPlayerCharacter* Player = Cast<APCPlayerCharacter>(OtherActor))
	{
		APCGameMode* GameMode = Cast<APCGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		GameMode->StopGameTimer();

		UPCGameInstance* GameInstance = Cast<UPCGameInstance>(GetGameInstance());
		if (GameInstance)
		{
			GameInstance->SetElapsedTime(GameMode->GetElapsedTime());
		}

		UGameplayStatics::OpenLevel(this, FName("VictoryScreen"));
	}
}

