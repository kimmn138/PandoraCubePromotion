// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PCCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APCCharacterBase::APCCharacterBase()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 540.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 100.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	// �𵨸� ������ ������⿡ ���缭 ��ġ�� ȸ�� ����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	// ĳ������ ���̷�Ż �޽� ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	// ĳ������ �ִϸ��̼� �ν��Ͻ� ����
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<APCWeaponBase> WeaponClassRef(TEXT("/Script/PandoraCube.PCWeaponBase"));
	if (nullptr != WeaponClassRef.Class)
	{
		WeaponClass = WeaponClassRef.Class;
	}
}

void APCCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponClass)
	{
		// WeaponBase �ν��Ͻ� ����
		EquippedWeapon = GetWorld()->SpawnActor<APCWeaponBase>(WeaponClass);

		if (EquippedWeapon)
		{
			// ���� ��Ģ�� ���� (�� ��Ģ�� ���� ��ġ�� ȸ���� ���Ͽ� �°� ������)
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

			// ĳ������ ���̷�Ż �޽� ������Ʈ�� WeaponBase ����
			EquippedWeapon->AttachToComponent(GetMesh(), AttachmentRules, TEXT("WeaponSocket"));

			// �ɼ�: WeaponBase�� ��� ��ġ�� ȸ���� ������ �� ����
			EquippedWeapon->SetActorRelativeLocation(FVector(-9.0f, 1.0f, 0.0f));
			EquippedWeapon->SetActorRelativeRotation(FRotator(10.79997f, 97.199968f, 3.59999f));
		}
	}
}

FTransform APCCharacterBase::GetLeftHandSocketTransform_Implementation() const
{
	if (EquippedWeapon)
	{
		APCWeaponBase* WeaponBase = Cast<APCWeaponBase>(EquippedWeapon);
		if (WeaponBase && WeaponBase->SkeletalMesh)
		{
			FTransform LHIKTransform = WeaponBase->SkeletalMesh->GetSocketTransform(TEXT("LHIK"), ERelativeTransformSpace::RTS_World);

			FVector InPosition;
			FRotator InRotation;
			InPosition = LHIKTransform.GetLocation();
			FQuat InRotationQuat = LHIKTransform.GetRotation();
			InRotation = InRotationQuat.Rotator();

			FVector OutLocation;
			FRotator OutRotation;
			GetMesh()->TransformToBoneSpace("hand_r", InPosition, InRotation, OutLocation, OutRotation);

			FTransform BoneSpaceTransform(OutRotation, OutLocation);

			return BoneSpaceTransform;
		}
	}

	return FTransform::Identity;
}


