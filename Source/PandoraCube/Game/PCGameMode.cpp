// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCGameMode.h"

APCGameMode::APCGameMode()
{
	// ������ Ŭ���� ������ �������� ���ؼ� ConstructorHelpers�� �����
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/PandoraCube.PCCharacterPlayer"));
	if (DefaultPawnClassRef.Class)
	{
		// ���Ӹ���� �⺻ ���� ABCharacterPlayer�� ������
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	// �⺻������ Ŭ���� �����̱� ������ _C�� ������ ����
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/PandoraCube.PCPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		// ���Ӹ���� �÷��̾� ��Ʈ�ѷ��� AABPlayerController�� ������
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
