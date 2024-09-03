// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PCCharacterPlayer.h"
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

APCCharacterPlayer::APCCharacterPlayer()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh(), FName("neck_02"));
	CameraBoom->TargetArmLength = 0.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->SetRelativeLocation(FVector(15.165182f, 0.0f, 9.059375f));
	FollowCamera->bUsePawnControlRotation = false;

	ControllerTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ControllerTimeline"));
	InterpFunction.BindUFunction(this, FName("HandleTimelineProgress"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> ControllerRecoilCurveRef(TEXT("/Script/Engine.CurveFloat'/Game/PandoraCube/Curve/ControllerCurveFloat.ControllerCurveFloat'"));
	if (nullptr != ControllerRecoilCurveRef.Object)
	{
		ControllerRecoilCurve = ControllerRecoilCurveRef.Object;
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

	SideMov = 0.0f;
	MouseX = 0.0f;
	MouseY = 0.0f;

	AnimInstanceRef = nullptr;
}

void APCCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

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
		ControllerTimeline->AddInterpFloat(ControllerRecoilCurve, InterpFunction);
		ControllerTimeline->SetTimelineLength(0.15f);
		ControllerTimeline->SetLooping(false);
	}
}

void APCCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APCCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APCCharacterPlayer::Look);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APCCharacterPlayer::Sprint);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APCCharacterPlayer::StopSprinting);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APCCharacterPlayer::Fire);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &APCCharacterPlayer::StopFiring);
	EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Started, this, &APCCharacterPlayer::Aiming);
	EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Completed, this, &APCCharacterPlayer::StopAiming);
}

FHandSwayValues APCCharacterPlayer::GetHandSwayFloats_Implementation() const
{
	FHandSwayValues SwayValues;

	SwayValues.SideMov = SideMov;
	SwayValues.MouseX = MouseX;
	SwayValues.MouseY = MouseY;

	return SwayValues;
}

void APCCharacterPlayer::SetHandSwayFloats(float& OutSideMov, float& OutMouseX, float& OutMouseY) const
{
	OutSideMov = CurrentSideMov;
	OutMouseX = CurrentMouseX;
	OutMouseY = CurrentMouseY;
}

void APCCharacterPlayer::Move(const FInputActionValue& Value)
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

void APCCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	CurrentMouseX = LookAxisVector.X;
	CurrentMouseY = LookAxisVector.Y;

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void APCCharacterPlayer::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void APCCharacterPlayer::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
}

void APCCharacterPlayer::Fire()
{
	FTimerHandle FireDelayHandle;
	bIsAttacking = 1;
		
	GetWorld()->GetTimerManager().SetTimer(
		FireDelayHandle,
		[this]()
		{
			if (bIsAttacking)
			{
				ShootRay();
				FOutputDeviceNull Ar;
				FString FunctionNameWithArgs = FString::Printf(TEXT("ProceduralRecoil %f"), 1.5);

				bool bSuccess = AnimInstanceRef->CallFunctionByNameWithArguments(*FunctionNameWithArgs, Ar, nullptr, true);
				if (bSuccess)
				{
					UGameplayStatics::PlaySoundAtLocation(this, RifleSound, FollowCamera->GetComponentLocation());
					ControllerRecoil();
				}
			}
		},
		FireRate,
		true
	);
}


void APCCharacterPlayer::StopFiring()
{
	bIsAttacking = 0;
}

void APCCharacterPlayer::Aiming()
{
	bIsAiming = 1;
}

void APCCharacterPlayer::StopAiming()
{
	bIsAiming = 0;
}

void APCCharacterPlayer::ShootRay()
{
	FVector Result = FollowCamera->GetForwardVector() * 50000.0f;

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
				Damage,     
				GetController(),   
				this,    
				nullptr    
			);
		}

		UGameplayStatics::ApplyDamage(
			HitActor,
			Damage,
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

void APCCharacterPlayer::ControllerRecoil()
{
	if (ControllerRecoilCurve && !ControllerTimeline->IsPlaying())
	{
		ControllerTimeline->PlayFromStart();
	}
}

void APCCharacterPlayer::HandleTimelineProgress(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Timeline Value: %f"), Value);
	float InterpolatedValue = FMath::Lerp(0.0f, RecoilAmount, Value);

	float SelectedPitch = bIsAiming ? 0.25 : 1.0;
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

