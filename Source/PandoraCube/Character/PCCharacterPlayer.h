// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PCCharacterBase.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "PCCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PANDORACUBE_API APCCharacterPlayer : public APCCharacterBase
{
	GENERATED_BODY()
	
public:
	APCCharacterPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FHandSwayValues  GetHandSwayFloats_Implementation() const override;
	void SetHandSwayFloats(float& OutSideMov, float& OutMouseX, float& OutMouseY) const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timeline, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTimelineComponent> ControllerTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timeline, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCurveFloat> ControllerRecoilCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimInstance> AnimInstanceRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<class UParticleSystem> WeaponMetalParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<class UParticleSystem> WeaponFleshParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<class UParticleSystem> WeaponOtherParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<class UParticleSystem> WeaponMuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundBase> RifleSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundBase> MetalHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundBase> FleshHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundBase> OtherHitSound;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AimingAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Sprint();
	void StopSprinting();
	void Fire();
	void StopFiring();
	void Aiming();
	void StopAiming();

	void ShootRay();

	void ControllerRecoil();

private:
	float CurrentSideMov = 0.0f;
	float CurrentMouseX = 0.0f;
	float CurrentMouseY = 0.0f;

	float SideMov = 0.0f;
	float MouseX = 0.0f;
	float MouseY = 0.0f;

	int32 bIsAttacking : 1;
	int32 bIsAiming : 1;

	float Damage;

	float FireRate = 0.2f;

	float RecoilAmount = 0.15f;

	FOnTimelineFloat InterpFunction{};

	UFUNCTION()
	void HandleTimelineProgress(float Value);
};
