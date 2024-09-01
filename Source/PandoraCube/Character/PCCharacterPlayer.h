// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PCCharacterBase.h"
#include "InputActionValue.h"
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
	// 매핑 컨텍스트를 할당하는 역할 진행
	virtual void BeginPlay() override;

public:
	// 언리얼 엔진의 인풋 시스템에서 액션과 Move와 Look 함수를 서로 매핑시켜주는 역할 진행
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Camera Section
protected:
	// 지지대 역할을 하는 컴포넌트, Meta는 private로 선언된 객체들을 블루프린트에서도 접근 가능하도록 만드는 지시자임
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	// 실제 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// Input Section
protected:
	// 1개의 매핑 컨텍스트와 3개의 액션 언리얼 오브젝트 선언
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

	// 기능을 구현할 2개의 함수
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Sprint();
	void StopSprinting();
};
