// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SetSpeed.h"
#include "AIController.h"
#include "PCAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_SetSpeed::UBTTask_SetSpeed()
{
	NodeName = TEXT("SetSpeed");
}

EBTNodeResult::Type UBTTask_SetSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    ACharacter* ControllingPawn = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (nullptr == ControllingPawn)
    {
        return EBTNodeResult::Failed;
    }

    UCharacterMovementComponent* MovementComponent = ControllingPawn->GetCharacterMovement();
    if (MovementComponent)
    {
        bool bIsChasing = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_CHASING);

        if (bIsChasing)
        {
            MovementComponent->MaxWalkSpeed = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_MAXSPEED);
        }
        else
        {
            MovementComponent->MaxWalkSpeed = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_SPEED); 
        }
    }

    return EBTNodeResult::Succeeded;
}
