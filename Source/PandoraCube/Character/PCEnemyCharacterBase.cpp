// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PCEnemyCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "PCComboActionData.h"
#include "Physics/PCCollision.h"
#include "Engine/DamageEvents.h"
#include "CharacterStat/PCCharacterStatComponent.h"
#include "AI/PCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/PCAI.h"
#include "PCPlayerCharacter.h"
#include "Physics/PCCollision.h"
#include "Components/AudioComponent.h"

// Sets default values
APCEnemyCharacterBase::APCEnemyCharacterBase()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = CurrentStats.Speed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->GravityScale = 5.0f; 
	GetCharacterMovement()->bMaintainHorizontalGroundVelocity = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("BlockAll"));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->bAutoActivate = false;

	Stat = CreateDefaultSubobject<UPCCharacterStatComponent>(TEXT("Stat"));

	AIControllerClass = APCAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<UDataTable> StatDataTableRef(TEXT("/Script/Engine.DataTable'/Game/PandoraCube/CharacterStat/DT_CharacterStat.DT_CharacterStat'"));
	if (nullptr != StatDataTableRef.Object)
	{
		StatDataTable = StatDataTableRef.Object;
	}

	Tags.Add(FName("Flesh"));
}

void APCEnemyCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &APCEnemyCharacterBase::SetDead);
}

void APCEnemyCharacterBase::TakeKnockBack(const FVector& HitLocation, const FVector& ImpactDirection, float Force)
{
	FVector KnockbackDirection = ImpactDirection;
	KnockbackDirection.Z = 0;
	KnockbackDirection.Normalize();

	FVector KnockbackForce = KnockbackDirection * Force;

	UE_LOG(LogTemp, Warning, TEXT("KnockBack!!"));
	LaunchCharacter(KnockbackForce, true, false);
}

void APCEnemyCharacterBase::SetImmediateChase(bool bChase)
{
	bImmediateChase = bChase;
	if (bChase)
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
			if (BlackboardComp)
			{
				BlackboardComp->SetValueAsBool(BBKEY_IMMEDIATE, true);
			}
		}
	}
}

void APCEnemyCharacterBase::Attack()
{
	if (!bIsDead)
	{
		ProcessComboCommand();
	}
}

void APCEnemyCharacterBase::ProcessComboCommand()
{
	if (bIsDead) return;

	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

void APCEnemyCharacterBase::ComboActionBegin()
{
	if (bIsDead) return;

	CurrentCombo = 1;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	const float AttackSpeedRate = 1.0f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &APCEnemyCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	UE_LOG(LogTemp, Warning, TEXT("Montage_Play: ComboActionMontage started successfully!"));

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void APCEnemyCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	if (bIsDead) return;

	if (TargetMontage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ComboActionEnd: TargetMontage is NULL!"));
		return;
	}

	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	UE_LOG(LogTemp, Warning, TEXT("ComboEnd!!!! Montage: %s, IsProperlyEnded: %s"),
		*TargetMontage->GetName(),
		IsProperlyEnded ? TEXT("True") : TEXT("False"));

	HasNextComboCommand = false;  
	NotifyComboActionEnd();
}

void APCEnemyCharacterBase::SetComboCheckTimer()
{
	if (bIsDead) return;

	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = CurrentStats.AttackRate;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;

	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &APCEnemyCharacterBase::ComboCheck, ComboEffectiveTime, false);
	}
}

void APCEnemyCharacterBase::ComboCheck()
{
	if (bIsDead) return;

	ComboTimerHandle.Invalidate();

	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (!AnimInstance || !AnimInstance->Montage_IsPlaying(ComboActionMontage))
		{
			UE_LOG(LogTemp, Error, TEXT("ComboCheck: ComboActionMontage is not playing!"));
			ComboActionEnd(ComboActionMontage, false);  
			return;
		}

		if (CurrentCombo >= ComboActionData->MaxComboCount)
		{
			UE_LOG(LogTemp, Warning, TEXT("ComboCheck: Reached last combo section, ending combo."));
			ComboActionEnd(ComboActionMontage, true);  
			return;
		}

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		SetComboCheckTimer();

		HasNextComboCommand = false;
	}
}


void APCEnemyCharacterBase::AttackHitCheck()
{
	if (bIsDead) return;

	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = CurrentStats.AttackRange;
	const float AttackRadius = CurrentStats.AttackRadius;
	const float AttackDamage = CurrentStats.Damage;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_PCACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		AActor* HitActor = OutHitResult.GetActor();
		if (HitActor && HitActor->IsA(APCPlayerCharacter::StaticClass()))
		{
			FDamageEvent DamageEvent;
			HitActor->TakeDamage(AttackDamage, DamageEvent, GetController(), this);

			HasNextComboCommand = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AttackHitCheck: Target is not a player, ignoring attack"));
		}
	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif
}

float APCEnemyCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!bIsDead)
	{
		PlayHitAnimation();
	}
	Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}

void APCEnemyCharacterBase::SetDead()
{
	bIsDead = true;

	if (AudioComponent->IsPlaying())
	{
		AudioComponent->Stop();
	}

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);

	APCAIController* AIController = Cast<APCAIController>(GetController());
	if (AIController)
	{
		AIController->BrainComponent->StopLogic(TEXT("Character is Dead"));
	}

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}

void APCEnemyCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

void APCEnemyCharacterBase::PlayHitAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	//AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(HitMontage, 1.0f);
}

float APCEnemyCharacterBase::GetAIPatrolRadius()
{
	return 900.0f;
}

float APCEnemyCharacterBase::GetAIDetectRange()
{
	return 2500.0f;
}

float APCEnemyCharacterBase::GetAIAttackRange()
{
	return CurrentStats.AttackRange + CurrentStats.AttackRadius * 2;
}

float APCEnemyCharacterBase::GetAITurnSpeed()
{
	return 2.0f;
}

bool APCEnemyCharacterBase::GetAIIsFleeing()
{
	return bIsFleeing;
}

void APCEnemyCharacterBase::SetAIIsFleeing(bool NewFleeing)
{
	bIsFleeing = NewFleeing;
}

USoundBase* APCEnemyCharacterBase::GetAISound()
{
	return CurrentStats.ZombieSound;
}

FVector APCEnemyCharacterBase::GetAILocation()
{
	return GetActorLocation();
}

UAudioComponent* APCEnemyCharacterBase::GetAIAudioComponent()
{
	return AudioComponent;
}

void APCEnemyCharacterBase::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void APCEnemyCharacterBase::AttackByAI()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && AnimInstance->Montage_IsPlaying(ComboActionMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackByAI: ComboActionMontage is already playing, skipping!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("DogAttack2!!!!"));
	ProcessComboCommand();
}

void APCEnemyCharacterBase::NotifyComboActionEnd()
{
	OnAttackFinished.ExecuteIfBound();
}

