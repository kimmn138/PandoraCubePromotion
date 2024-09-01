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
	// ���� ���ؽ�Ʈ�� �Ҵ��ϴ� ���� ����
	virtual void BeginPlay() override;

public:
	// �𸮾� ������ ��ǲ �ý��ۿ��� �׼ǰ� Move�� Look �Լ��� ���� ���ν����ִ� ���� ����
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Camera Section
protected:
	// ������ ������ �ϴ� ������Ʈ, Meta�� private�� ����� ��ü���� �������Ʈ������ ���� �����ϵ��� ����� ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	// ���� ī�޶� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// Input Section
protected:
	// 1���� ���� ���ؽ�Ʈ�� 3���� �׼� �𸮾� ������Ʈ ����
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

	// ����� ������ 2���� �Լ�
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Sprint();
	void StopSprinting();
};
