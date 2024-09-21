// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PCPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Misc/OutputDeviceNull.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Prop/PCBlood.h"
#include "Blueprint/UserWidget.h"
#include "Item/ItemTypes.h"
#include "PickUps/PCPickUpBase.h"

APCPlayerCharacter::APCPlayerCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(41.4f, 90.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 540.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 100.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh(), FName("neck_02"));
	CameraBoom->TargetArmLength = 0.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->SetRelativeLocation(FVector(15.165182f, 0.0f, 9.059375f));
	FollowCamera->bUsePawnControlRotation = false;

	ControllerTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ControllerTimeline"));
	ControllerRecoilInterpFunction.BindUFunction(this, FName("HandleTimelineProgress"));
	
	AimTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AimTimeline"));
	AimInterpFunction.BindUFunction(this, FName("SetCameraLocation"));

	InventoryComponent = CreateDefaultSubobject<UPCInventoryComponent>(TEXT("Inventory"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

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

	static ConstructorHelpers::FObjectFinder<UCurveFloat> ControllerRecoilCurveRef(TEXT("/Script/Engine.CurveFloat'/Game/PandoraCube/Curve/ControllerCurveFloat.ControllerCurveFloat'"));
	if (nullptr != ControllerRecoilCurveRef.Object)
	{
		ControllerRecoilCurve = ControllerRecoilCurveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> AimCurveRef(TEXT("/Script/Engine.CurveFloat'/Game/PandoraCube/Curve/AimCurveFloat.AimCurveFloat'"));
	if (nullptr != AimCurveRef.Object)
	{
		AimCurve = AimCurveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/PandoraCube/Input/IMC_Default.IMC_Default'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PandoraCube/Input/Actions/IA_Move.IA_Move'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PandoraCube/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PandoraCube/Input/Actions/IA_Look.IA_Look'"));
	if (nullptr != InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSprintRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PandoraCube/Input/Actions/IA_Sprint.IA_Sprint'"));
	if (nullptr != InputActionSprintRef.Object)
	{
		SprintAction = InputActionSprintRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionFireRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PandoraCube/Input/Actions/IA_Fire.IA_Fire'"));
	if (nullptr != InputActionFireRef.Object)
	{
		FireAction = InputActionFireRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAimingRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PandoraCube/Input/Actions/IA_Aim.IA_Aim'"));
	if (nullptr != InputActionAimingRef.Object)
	{
		AimingAction = InputActionAimingRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionChangeInven1Ref(TEXT("/Script/EnhancedInput.InputAction'/Game/PandoraCube/Input/Actions/IA_Inventory1.IA_Inventory1'"));
	if (nullptr != InputActionChangeInven1Ref.Object)
	{
		ChangeInven1Action = InputActionChangeInven1Ref.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionChangeInven2Ref(TEXT("/Script/EnhancedInput.InputAction'/Game/PandoraCube/Input/Actions/IA_Inventory2.IA_Inventory2'"));
	if (nullptr != InputActionChangeInven2Ref.Object)
	{
		ChangeInven2Action = InputActionChangeInven2Ref.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionReloadRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PandoraCube/Input/Actions/IA_Reload.IA_Reload'"));
	if (nullptr != InputActionReloadRef.Object)
	{
		ReloadAction = InputActionReloadRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionDropItemRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PandoraCube/Input/Actions/IA_DropItem.IA_DropItem'"));
	if (nullptr != InputActionDropItemRef.Object)
	{
		DropItemAction = InputActionDropItemRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLeanLeftRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PandoraCube/Input/Actions/IA_LeanLeft.IA_LeanLeft'"));
	if (nullptr != InputActionLeanLeftRef.Object)
	{
		LeanLeftAction = InputActionLeanLeftRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLeanRightRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PandoraCube/Input/Actions/IA_LeanRight.IA_LeanRight'"));
	if (nullptr != InputActionLeanRightRef.Object)
	{
		LeanRightAction = InputActionLeanRightRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> MetalParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/MilitaryWeapSilver/FX/P_Impact_Metal_Small_01.P_Impact_Metal_Small_01'"));
	if (nullptr != MetalParticleRef.Object)
	{
		WeaponMetalParticle = MetalParticleRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FleshParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/MilitaryWeapSilver/FX/P_Impact_Wood_Small_01.P_Impact_Wood_Small_01'"));
	if (nullptr != FleshParticleRef.Object)
	{
		WeaponFleshParticle = FleshParticleRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> OtherParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/MilitaryWeapSilver/FX/P_Impact_Stone_Large_01.P_Impact_Stone_Large_01'"));
	if (nullptr != OtherParticleRef.Object)
	{
		WeaponOtherParticle = OtherParticleRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> MuzzleFlashRef(TEXT("/Script/Engine.ParticleSystem'/Game/MilitaryWeapSilver/FX/P_AssaultRifle_MuzzleFlash.P_AssaultRifle_MuzzleFlash'"));
	if (nullptr != MuzzleFlashRef.Object)
	{
		WeaponMuzzleFlash = MuzzleFlashRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> RifleSoundRef(TEXT("/Script/Engine.SoundCue'/Game/MilitaryWeapSilver/Sound/Rifle/Cues/RifleA_Fire_Cue.RifleA_Fire_Cue'"));
	if (nullptr != RifleSoundRef.Object)
	{
		RifleSound = RifleSoundRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> MetalHitSoundRef(TEXT("/Script/Engine.SoundCue'/Game/MilitaryWeapSilver/Sound/Shotgun/Cues/Shotgun_ImpactSurface_Cue.Shotgun_ImpactSurface_Cue'"));
	if (nullptr != MetalHitSoundRef.Object)
	{
		MetalHitSound = MetalHitSoundRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> FleshHitSoundRef(TEXT("/Script/Engine.SoundCue'/Game/MilitaryWeapSilver/Sound/Rifle/Cues/Rifle_ImpactBody_Cue.Rifle_ImpactBody_Cue'"));
	if (nullptr != FleshHitSoundRef.Object)
	{
		FleshHitSound = FleshHitSoundRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> OtherHitSoundRef(TEXT("/Script/Engine.SoundCue'/Game/MilitaryWeapSilver/Sound/SniperRifle/Cues/SniperRifle_ImpactSurface_Cue.SniperRifle_ImpactSurface_Cue'"));
	if (nullptr != OtherHitSoundRef.Object)
	{
		OtherHitSound = OtherHitSoundRef.Object;
	}

	static ConstructorHelpers::FClassFinder<APCBlood> BloodDecalRef(TEXT("/Script/CoreUObject.Class'/Script/PandoraCube.PCBlood'"));
	if (nullptr != BloodDecalRef.Class)
	{
		BloodDecal = BloodDecalRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerMainWidgetRef(TEXT("/Game/PandoraCube/Blueprints/Widget/PlayerMainWidget.PlayerMainWidget_C"));
	if (PlayerMainWidgetRef.Class)
	{
		UUserWidget* PlayerWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), PlayerMainWidgetRef.Class);
		if (PlayerWidgetInstance)
		{
			PlayerWidgetInstance->AddToViewport();
		}
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableRef(TEXT("/Script/Engine.DataTable'/Game/PandoraCube/ItemData/ItemDataTable.ItemDataTable'"));
	if (nullptr != ItemDataTableRef.Object)
	{
		ItemDataTable = ItemDataTableRef.Object;
	}

	SideMov = 0.0f;
	MouseX = 0.0f;
	MouseY = 0.0f;

	AnimInstanceRef = nullptr;

	bCanAim = 1;
	bCanFire = 1;
	bStopLeftHandIK = 0;

	CurrentItemSelection = 0;
}

//void APCPlayerCharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	Aiming();
//}

void APCPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Flesh"));
	Tags.Add(FName("Player"));

	EquipItem();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	FTimerHandle SwayTimer;

	GetWorldTimerManager().SetTimer(SwayTimer, [this]() {
		SetHandSwayFloats(SideMov, MouseX, MouseY);
		}, 0.015f, true);

	AnimInstanceRef = GetMesh()->GetAnimInstance();
	check(AnimInstanceRef != nullptr && "Failed to get Animation instance. AnimInstanceRef is null!");

	if (ControllerRecoilCurve)
	{
		ControllerTimeline->AddInterpFloat(ControllerRecoilCurve, ControllerRecoilInterpFunction);
		ControllerTimeline->SetTimelineLength(0.15f);
		ControllerTimeline->SetLooping(false);
	}

	if (AimCurve)
	{
		AimTimeline->AddInterpFloat(AimCurve, AimInterpFunction);
		AimTimeline->SetTimelineLength(0.25f);
		AimTimeline->SetLooping(false);
	}

	FTimerHandle CheckWallTimer;

	GetWorld()->GetTimerManager().SetTimer(CheckWallTimer, this, &APCPlayerCharacter::CheckWallTick, 0.02f, true);
}

void APCPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APCPlayerCharacter::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APCPlayerCharacter::Look);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APCPlayerCharacter::Sprint);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APCPlayerCharacter::StopSprinting);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APCPlayerCharacter::StartFiring);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &APCPlayerCharacter::StopFiring);
	EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Started, this, &APCPlayerCharacter::Aiming);
	EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Completed, this, &APCPlayerCharacter::StopAiming);
	EnhancedInputComponent->BindAction(ChangeInven1Action, ETriggerEvent::Started, this, &APCPlayerCharacter::ChangeInven1);
	EnhancedInputComponent->BindAction(ChangeInven2Action, ETriggerEvent::Started, this, &APCPlayerCharacter::ChangeInven2);
	EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &APCPlayerCharacter::Reload);
	EnhancedInputComponent->BindAction(DropItemAction, ETriggerEvent::Started, this, &APCPlayerCharacter::DropItem);
	EnhancedInputComponent->BindAction(LeanLeftAction, ETriggerEvent::Triggered, this, &APCPlayerCharacter::LeanLeftPressed);
	EnhancedInputComponent->BindAction(LeanLeftAction, ETriggerEvent::Completed, this, &APCPlayerCharacter::LeanLeftReleased);
	EnhancedInputComponent->BindAction(LeanRightAction, ETriggerEvent::Triggered, this, &APCPlayerCharacter::LeanRightPressed);
	EnhancedInputComponent->BindAction(LeanRightAction, ETriggerEvent::Completed, this, &APCPlayerCharacter::LeanRightReleased);
}

FHandSwayValues APCPlayerCharacter::GetHandSwayFloats_Implementation() const
{
	FHandSwayValues SwayValues;

	SwayValues.SideMov = SideMov;
	SwayValues.MouseX = MouseX;
	SwayValues.MouseY = MouseY;

	return SwayValues;
}

void APCPlayerCharacter::SetHandSwayFloats(float& OutSideMov, float& OutMouseX, float& OutMouseY) const
{
	OutSideMov = CurrentSideMov;
	OutMouseX = CurrentMouseX;
	OutMouseY = CurrentMouseY;
}

FTransform APCPlayerCharacter::GetLeftHandSocketTransform_Implementation() const
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

void APCPlayerCharacter::ReduceBullet()
{
	InventoryComponent->Inventory[CurrentItemSelection].Bullets -= 1;
}

bool APCPlayerCharacter::BulletsLeft()
{
	if (InventoryComponent->Inventory.IsValidIndex(CurrentItemSelection))
	{
		if (InventoryComponent->Inventory[CurrentItemSelection].Bullets >= 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

float APCPlayerCharacter::GetWallDistance_Implementation() const
{
	return WallDistance;
}

void APCPlayerCharacter::SetCameraLocation(float Value)
{
	AimAlpha = Value;

	FVector LerpVector(1.0f, 4.5f, 13.0f);
	if (EquippedWeapon)
	{
		USceneComponent* AimOffsetComponent = Cast<USceneComponent>(EquippedWeapon->GetDefaultSubobjectByName(TEXT("AimOffset")));
		FVector AimOffsetLocation = AimOffsetComponent->GetRelativeLocation();

		FVector InterpolatedValue = FMath::Lerp(FVector(0.0f, 0.0f, 0.0f), LerpVector + AimOffsetLocation, Value);

		CameraBoom->SetRelativeLocation(InterpolatedValue);
	}
}

bool APCPlayerCharacter::GetIsAim_Implementation() const
{
	return bIsAiming;
}

void APCPlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	CurrentSideMov = MovementVector.Y;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void APCPlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	CurrentMouseX = LookAxisVector.X;
	CurrentMouseY = LookAxisVector.Y;

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void APCPlayerCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	StopAiming();
	bCanAim = 0;
}

void APCPlayerCharacter::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	bCanAim = 1;
}

void APCPlayerCharacter::StartFiring()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(FireDelayHandle))
	{
		bIsAttacking = 1;
		Fire();
	}
}

void APCPlayerCharacter::Fire()
{
	switch (WeaponType)
	{
		case EItemType::IT_Rifle:
			if (bIsAttacking && bCanFire)
			{
				if (BulletsLeft())
				{
					FString NumberAsString = FString::FromInt(InventoryComponent->Inventory[CurrentItemSelection].Bullets);
					UKismetSystemLibrary::PrintString(GetWorld(), NumberAsString, true, true, FColor::Green, 2.0f);
					ReduceBullet();
					ShootRay();
					FOutputDeviceNull Ar;
					FString FunctionNameWithArgs = FString::Printf(TEXT("ProceduralRecoil %f"), CurrentStats.ProceduralRecoil);

					bool bSuccess = AnimInstanceRef->CallFunctionByNameWithArguments(*FunctionNameWithArgs, Ar, nullptr, true);
					if (bSuccess)
					{
						UGameplayStatics::PlaySoundAtLocation(this, RifleSound, FollowCamera->GetComponentLocation());
						ControllerRecoil();

						if (EquippedWeapon)
						{
							USkeletalMeshComponent* WeaponMesh = EquippedWeapon->FindComponentByClass<USkeletalMeshComponent>();
							FTransform SocketTransform = WeaponMesh->GetSocketTransform(TEXT("MuzzleFlash"), RTS_World);

							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponMuzzleFlash, SocketTransform.GetLocation(), SocketTransform.GetRotation().Rotator());
						}
					}

					GetWorld()->GetTimerManager().SetTimer(
						FireDelayHandle,
						this,
						&APCPlayerCharacter::Fire,
						CurrentStats.FireRate,
						false
					);
				}
			}
			break;

		case EItemType::IT_Pistol:
			if (bIsAttacking && bCanFire)
			{
				if (BulletsLeft())
				{
					FString NumberAsString = FString::FromInt(InventoryComponent->Inventory[CurrentItemSelection].Bullets);
					UKismetSystemLibrary::PrintString(GetWorld(), NumberAsString, true, true, FColor::Green, 2.0f);
					ReduceBullet();
					ShootRay();
					FOutputDeviceNull Ar;
					FString FunctionNameWithArgs = FString::Printf(TEXT("ProceduralRecoil %f"), CurrentStats.ProceduralRecoil);

					bool bSuccess = AnimInstanceRef->CallFunctionByNameWithArguments(*FunctionNameWithArgs, Ar, nullptr, true);
					if (bSuccess)
					{
						UGameplayStatics::PlaySoundAtLocation(this, RifleSound, FollowCamera->GetComponentLocation());
						ControllerRecoil();

						if (EquippedWeapon)
						{
							USkeletalMeshComponent* WeaponMesh = EquippedWeapon->FindComponentByClass<USkeletalMeshComponent>();
							FTransform SocketTransform = WeaponMesh->GetSocketTransform(TEXT("MuzzleFlash"), RTS_World);

							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponMuzzleFlash, SocketTransform.GetLocation(), SocketTransform.GetRotation().Rotator());
						}
					}
				}
			}
			break;

		default:
			break;
	}
}

void APCPlayerCharacter::StopFiring()
{
	bIsAttacking = 0;
	GetWorld()->GetTimerManager().ClearTimer(FireDelayHandle);
}

void APCPlayerCharacter::Aiming()
{
	if (bCanAim)
	{
		bIsAiming = 1;
		if (AimCurve)
		{
			AimTimeline->Play();
		}
	}
}

void APCPlayerCharacter::StopAiming()
{
	bIsAiming = 0;
	if (AimCurve)
	{
		AimTimeline->Reverse();
	}
}

void APCPlayerCharacter::ChangeInven1()
{
	CurrentItemSelection = 0;
	EquipItem();
}

void APCPlayerCharacter::ChangeInven2()
{
	CurrentItemSelection = 1;
	EquipItem();
}

void APCPlayerCharacter::Reload()
{
	if (InventoryComponent->Inventory[CurrentItemSelection].Bullets < CurrentStats.MagSize)
	{
		bIsAiming = 0;
		bCanAim = 0;
		bCanFire = 0;
		bStopLeftHandIK = 1;

		AnimInstanceRef->Montage_Play(CurrentReloadAnimation, 1.0f);

		FTimerHandle ReloadTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &APCPlayerCharacter::CompleteReload, CurrentStats.ReloadTime, false);
	}
}

void APCPlayerCharacter::DropItem()
{
	if (InventoryComponent->Inventory.IsValidIndex(CurrentItemSelection))
	{
		if (!(InventoryComponent->Inventory[CurrentItemSelection].ID <= 0))
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			FTransform DropTransform;
			DropTransform.SetLocation(FollowCamera->GetForwardVector() * 200 + FollowCamera->GetComponentLocation());
			APCPickUpBase* SpawnedPickup = GetWorld()->SpawnActor<APCPickUpBase>(CurrentWeaponPickupClass, DropTransform, SpawnParams);

			if (SpawnedPickup)
			{
				SpawnedPickup->Item = InventoryComponent->Inventory[CurrentItemSelection];
				InventoryComponent->Inventory.RemoveAt(CurrentItemSelection);
				CurrentItemSelection = 0;
				EquipItem();
			}
		}
	}
}

void APCPlayerCharacter::LeanLeftPressed()
{
	bLeanLeft = 1;
}

void APCPlayerCharacter::LeanLeftReleased()
{
	bLeanLeft = 0;
}

void APCPlayerCharacter::LeanRightPressed()
{
	bLeanRight = 1;
}

void APCPlayerCharacter::LeanRightReleased()
{
	bLeanRight = 0;
}

void APCPlayerCharacter::CompleteReload()
{
	bCanAim = 1;
	bCanFire = 1;
	InventoryComponent->Inventory[CurrentItemSelection].Bullets = CurrentStats.MagSize;
	bStopLeftHandIK = 0;
}

void APCPlayerCharacter::CheckWallTick()
{
	ECollisionChannel TraceChannel = ECC_Camera;

	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	FCollisionQueryParams TraceParams(FName(TEXT("")), false);
	TraceParams.AddIgnoredActors(ActorsToIgnore);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		FollowCamera->GetComponentLocation(),
		FollowCamera->GetForwardVector() * MaxWallDistance + FollowCamera->GetComponentLocation(),
		TraceChannel,
		TraceParams
	);

	if (bHit)
	{
		FVector HitLocation = HitResult.Location;
		float Distance = FVector::Dist(HitLocation, FollowCamera->GetComponentLocation());
		Distance /= MaxWallDistance;
		WallDistance = Distance;
	}
	else
	{
		WallDistance = 1.0f;
	}
}

void APCPlayerCharacter::ShootRay()
{
	FVector Result = FollowCamera->GetForwardVector() * CurrentStats.Range;

	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + Result;

	ECollisionChannel TraceChannel = ECC_Camera;

	FHitResult HitResult;

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		TraceChannel,
		TraceParams
	);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		FVector HitLocation = HitResult.ImpactPoint;

		if (HitActor)
		{

			bool bParticleSpawned = false;

			for (const FName& Tag : HitActor->Tags)
			{

				if (Tag == "Metal")
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponMetalParticle, HitLocation, FRotator::ZeroRotator);
					UGameplayStatics::PlaySoundAtLocation(this, MetalHitSound, HitLocation);
					bParticleSpawned = true;
					break;
				}
				else if (Tag == "Flesh")
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponFleshParticle, HitLocation, FRotator::ZeroRotator);
					UGameplayStatics::PlaySoundAtLocation(this, FleshHitSound, HitLocation);
					bParticleSpawned = true;

					FCollisionQueryParams NotEnemyTraceParams;
					NotEnemyTraceParams.AddIgnoredActor(this);
					NotEnemyTraceParams.AddIgnoredActor(HitActor);

					FHitResult HitEnemyResult;
					FVector EndPoint = FollowCamera->GetForwardVector() * 1500 + HitLocation;

					bool bEnemyHit = GetWorld()->LineTraceSingleByChannel(
						HitEnemyResult,
						HitLocation,
						EndPoint,
						ECC_Visibility,
						NotEnemyTraceParams
					);

					if (bEnemyHit)
					{
						FVector HitNormal = HitEnemyResult.Normal;
						FRotator RotationFromNormal = FRotationMatrix::MakeFromX(HitNormal).Rotator();

						FTransform BloodTransform(RotationFromNormal, HitEnemyResult.ImpactPoint);

						FActorSpawnParameters SpawnParams;
						SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

						APCBlood* SpawnedBloodDecal = GetWorld()->SpawnActor<APCBlood>(BloodDecal, BloodTransform, SpawnParams);
						check(SpawnedBloodDecal != nullptr);
					}

					break;
				}
			}

			if (!bParticleSpawned)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponOtherParticle, HitLocation, FRotator::ZeroRotator);
				UGameplayStatics::PlaySoundAtLocation(this, OtherHitSound, HitLocation);
			}

			UGameplayStatics::ApplyDamage(
				HitActor,
				CurrentStats.Damage,
				GetController(),
				this,
				nullptr
			);
		}

		UGameplayStatics::ApplyDamage(
			HitActor,
			CurrentStats.Damage,
			GetController(),
			this,
			nullptr
		);
	}

	FColor LineColor = bHit ? FColor::Green : FColor::Red;

	/*DrawDebugLine(
		GetWorld(),
		Start,
		End,
		LineColor,
		true,
		0.0f,
		0,
		5.0f
	);*/
}

void APCPlayerCharacter::ControllerRecoil()
{
	if (ControllerRecoilCurve && !ControllerTimeline->IsPlaying())
	{
		ControllerTimeline->PlayFromStart();
	}
}

void APCPlayerCharacter::GetLeanBooleans_Implementation(bool& bLeft, bool& bRight)
{
	bLeft = bLeanLeft;
	bRight = bLeanRight;
}

bool APCPlayerCharacter::GetStopLeftHandIK_Implementation() const
{
	return bStopLeftHandIK;
}

void APCPlayerCharacter::HandleTimelineProgress(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Timeline Value: %f"), Value);
	float InterpolatedValue = FMath::Lerp(0.0f, CurrentStats.InputRecoil, Value);

	float SelectedPitch = bIsAiming ? -0.25 : -1.0;
	float ControllerPitchValue = InterpolatedValue * SelectedPitch;

	AddControllerPitchInput(ControllerPitchValue);

	float SelectedFloat = bIsAiming ? 1 : 2.5;
	float ControllerYawValue = InterpolatedValue / SelectedFloat;

	int32 RandomNumber = FMath::RandRange(1, 10);

	if (RandomNumber < 5)
	{
		AddControllerYawInput(ControllerYawValue * -1.0f);
	}
	else
	{
		AddControllerYawInput(ControllerYawValue);
	}
}

float APCPlayerCharacter::GetAimAlpha_Implementation() const
{
	return AimAlpha;
}

void APCPlayerCharacter::EquipItem()
{
	if (IsLocallyControlled())
	{
		if (InventoryComponent && InventoryComponent->Inventory.IsValidIndex(CurrentItemSelection))
		{
			int32 SelectedItem = InventoryComponent->Inventory[CurrentItemSelection].ID;
			UE_LOG(LogTemp, Warning, TEXT("Selected Item ID: %d"), SelectedItem);  // 로그로 SelectedItem 값 확인

			FName RowName = FName(*FString::FromInt(SelectedItem));
			FString ContextString = TEXT("Item Data Context");

			if (ItemDataTable)
			{
				FInventoryItem* Row = ItemDataTable->FindRow<FInventoryItem>(RowName, ContextString);

				if (Row)
				{
					WeaponClass = Row->WeaponClass;

					if (WeaponClass)
					{
						if (EquippedWeapon != nullptr)
						{
							EquippedWeapon->Destroy();
							EquippedWeapon = nullptr;
						}

						EquippedWeapon = GetWorld()->SpawnActor<APCWeaponBase>(WeaponClass);

						if (EquippedWeapon)
						{
							FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
							EquippedWeapon->AttachToComponent(GetMesh(), AttachmentRules, TEXT("WeaponSocket"));

							EquippedWeapon->SetActorRelativeLocation(FVector(-9.0f, 0.0f, -0.2f));
							EquippedWeapon->SetActorRelativeRotation(FRotator(16.5f, 93.5999f, 357.2f));

							CurrentAnimState = Row->AnimState;
							CurrentStats = Row->Stats;
							CurrentReloadAnimation = Row->ReloadAnimation;
							CurrentWeaponPickupClass = Row->PickupClass;
							WeaponType = Row->Type;
						}
					}
				}
			}
		}
		else
		{
			if (EquippedWeapon != nullptr)
			{
				EquippedWeapon->Destroy();
				EquippedWeapon = nullptr;
			}

			CurrentAnimState = EAnimState::Hands;
		}
	}
}


void APCPlayerCharacter::AddItemToInventory_Implementation(AActor* PickUp, FDynamicInventoryItem& Item)
{
	if (IsLocallyControlled())
	{
		if (InventoryComponent->Inventory.Num() >= InventoryComponent->MaxItemCount)
		{

		}
		else
		{
			InventoryComponent->Inventory.Add(Item);
			PickUp->Destroy();
			CurrentItemSelection = 0;
			EquipItem();
		}
	}
}

EAnimState APCPlayerCharacter::GetAnimState_Implementation() const
{
	return CurrentAnimState;
}
