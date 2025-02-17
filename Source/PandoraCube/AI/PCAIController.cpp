// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PCAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/PCAIControllerInterface.h"
#include "PCAI.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Character/PCEnemyCharacterBase.h"

APCAIController::APCAIController()
{
}

void APCAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());
		Blackboard->SetValueAsFloat(BBKEY_SPEED, CharacterStats.Speed);
		Blackboard->SetValueAsFloat(BBKEY_MAXSPEED, CharacterStats.MaxSpeed);

		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void APCAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void APCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	IPCAIControllerInterface* AICharacter = Cast<IPCAIControllerInterface>(InPawn);
	if (AICharacter)
	{
		BBAsset = AICharacter->GetBlackboardData();
		BTAsset = AICharacter->GetBehaviorTree();
		CharacterStats = AICharacter->GetCharacterStats();
	}

	RunAI();
}
