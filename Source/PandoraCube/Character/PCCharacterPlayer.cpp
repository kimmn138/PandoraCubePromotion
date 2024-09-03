// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PCCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Misc/OutputDeviceNull.h"

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

	FTimerHandle TimerHandle;

	GetWorldTimerManager().SetTimer(TimerHandle, [this]() {
			SetHandSwayFloats(SideMov, MouseX, MouseY);
	}, 0.015f, true);

	AnimInstanceRef = GetMesh()->GetAnimInstance();
	check(AnimInstanceRef != nullptr && "Failed to get Animation instance. AnimInstanceRef is null!");
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
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &APCCharacterPlayer::Fire);
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
	FOutputDeviceNull Ar;
	FString FunctionNameWithArgs = FString::Printf(TEXT("ProceduralRecoil %f"), 1.5);

	bool bSuccess = AnimInstanceRef->CallFunctionByNameWithArguments(*FunctionNameWithArgs, Ar, nullptr, true);

	if (bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Successfully called %s in AnimInstance"), *FunctionNameWithArgs);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to call %s in AnimInstance"), *FunctionNameWithArgs);
	}
}
