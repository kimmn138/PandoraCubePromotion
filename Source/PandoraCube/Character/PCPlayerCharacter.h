// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon/PCWeaponBase.h"
#include "Interface/PCPlayerInterface.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "Component/PCInventoryComponent.h"
#include "Engine/DataTable.h"
#include "Item/ItemTypes.h"
#include "Animation/AnimMontage.h"
#include "PCPlayerCharacter.generated.h"

UCLASS()
class PANDORACUBE_API APCPlayerCharacter : public ACharacter, public IPCPlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APCPlayerCharacter();

	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

// Animation Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimInstance> AnimInstanceRef;

// Attack Hit Section
protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetDead();

// Hand Move Section
public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FHandSwayValues  GetHandSwayFloats_Implementation() const override;

	UFUNCTION()
	void SetHandSwayFloats(float& OutSideMov, float& OutMouseX, float& OutMouseY) const;

// Weapon Section
protected:
	virtual FTransform GetLeftHandSocketTransform_Implementation() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class APCWeaponBase> WeaponClass;

	UPROPERTY()
	TObjectPtr<APCWeaponBase> EquippedWeapon;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	TSubclassOf<class APCBlood> BloodDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	TSubclassOf<class APCBulletHole> BulletHoleDecal;

	FTimerHandle FireDelayHandle;

	void ReduceBullet();

	bool BulletsLeft();

	float MaxWallDistance = 100.0f;
	virtual float GetWallDistance_Implementation() const override;

	uint8 bCooling : 1;

	UFUNCTION()
	void ResetFire();

// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// Input Section
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeInven1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeInven2Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DropItemAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LeanLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LeanRightAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Sprint();
	void StopSprinting();
	void StartFiring();
	UFUNCTION()
	void Fire();
	void StopFiring();
	void Aiming();
	void StopAiming();
	void ChangeInven1();
	void ChangeInven2();
	void Reload();
	void DropItem();
	void LeanLeftPressed();
	void LeanLeftReleased();
	void LeanRightPressed();
	void LeanRightReleased();

	UFUNCTION()
	void CompleteReload();

	UFUNCTION()
	void CheckWallTick();

	float WallDistance;

// Weapon Moving Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timeline, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTimelineComponent> ControllerTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timeline, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCurveFloat> ControllerRecoilCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timeline, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTimelineComponent> AimTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timeline, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCurveFloat> AimCurve;

	FOnTimelineFloat AimInterpFunction{};

	UFUNCTION()
	void SetCameraLocation(float Value);

	virtual bool  GetIsAim_Implementation() const override;

	void ShootRay();
	void ShotgunShootRay();

	void ControllerRecoil();

	float CurrentSideMov = 0.0f;
	float CurrentMouseX = 0.0f;
	float CurrentMouseY = 0.0f;

	float SideMov = 0.0f;
	float MouseX = 0.0f;
	float MouseY = 0.0f;

	uint8 bIsAttacking : 1;
	uint8 bIsAiming : 1;
	uint8 bCanAim : 1;
	uint8 bCanFire : 1;
	uint8 bStopLeftHandIK : 1;
	uint8 bLeanLeft : 1;
	uint8 bLeanRight : 1;

	virtual void GetLeanBooleans_Implementation(bool& bLeft, bool& bRight) override;

	virtual bool GetStopLeftHandIK_Implementation() const override;

	FOnTimelineFloat ControllerRecoilInterpFunction{};

	UFUNCTION()
	void HandleTimelineProgress(float Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = "true"))
	float AimAlpha;
	float AimCurrent;

	virtual float GetAimAlpha_Implementation() const override;

// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPCCharacterStatComponent> Stat;

// UI Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPCPlayerMainWidget> PlayerMainWidget;

	void SetupCharacterWidget();

// Inventory Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPCInventoryComponent> InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> ItemDataTable;

	void EquipItem();
	int32 CurrentItemSelection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data, Meta = (AllowPrivateAccess = "true"))
	FWeaponStats CurrentStats;

	virtual void AddItemToInventory_Implementation(AActor* PickUp, FDynamicInventoryItem& Item) override;

	TSubclassOf<class APCPickUpBase> CurrentWeaponPickupClass;

	EWeaponType WeaponType = EWeaponType::IT_Hand;

	EItemType ItemType;

// Animation Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> CurrentReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	EAnimState CurrentAnimState;

	virtual EAnimState GetAnimState_Implementation() const override;
};
