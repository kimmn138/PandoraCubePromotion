// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "PCAI.h"
#include "AIController.h"
#include "Interface/PCCharacterInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Physics/PCCollision.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Character/PCPlayerCharacter.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	IPCCharacterInterface* AIPawn = Cast<IPCCharacterInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	bool bIsScreaming = false;

	float DetectRadius = AIPawn->GetAIDetectRange();
	USoundBase* ZombieSound = AIPawn->GetAISound();
	UAudioComponent* ZombieAudioComponent = AIPawn->GetAIAudioComponent();

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		bool bImmediateChase = BlackboardComp->GetValueAsBool(BBKEY_IMMEDIATE);
		bIsScreaming = BlackboardComp->GetValueAsBool(BBKEY_SCREAMING);

		if (bImmediateChase)
		{
			APCPlayerCharacter* PlayerPawn = Cast<APCPlayerCharacter>(UGameplayStatics::GetPlayerPawn(ControllingPawn->GetWorld(), 0));
			if (PlayerPawn)
			{
				BlackboardComp->SetValueAsObject(BBKEY_TARGET, PlayerPawn);
				BlackboardComp->SetValueAsBool(BBKEY_CHASING, true);
				BlackboardComp->SetValueAsBool(BBKEY_SCREAMING, true);

				/*if (ZombieAudioComponent->IsPlaying())
				{
					ZombieAudioComponent->Stop();
				}
				ZombieAudioComponent->SetSound(ZombieSound);
				ZombieAudioComponent->Play();*/

				return;
			}
		}
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_PCACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_CHASING, true);
				if (!bIsScreaming)
				{
					if (ZombieAudioComponent->IsPlaying())
					{
						ZombieAudioComponent->Stop();
					}
					ZombieAudioComponent->SetSound(ZombieSound);
					ZombieAudioComponent->Play();

					bIsScreaming = true;
					BlackboardComp->SetValueAsBool(BBKEY_SCREAMING, true);
				}
				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_SCREAMING, false);
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_CHASING, false);
}
