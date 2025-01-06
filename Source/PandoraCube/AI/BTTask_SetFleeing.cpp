// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SetFleeing.h"
#include "PCAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/PCCharacterInterface.h"

UBTTask_SetFleeing::UBTTask_SetFleeing()
{
	NodeName = TEXT("SetFleeing");
}

EBTNodeResult::Type UBTTask_SetFleeing::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IPCCharacterInterface* AIPawn = Cast<IPCCharacterInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	bool IsFleeing = AIPawn->GetAIIsFleeing();
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_FLEEING, !IsFleeing);
	AIPawn->SetAIIsFleeing(!IsFleeing);

	return EBTNodeResult::Succeeded;
}
