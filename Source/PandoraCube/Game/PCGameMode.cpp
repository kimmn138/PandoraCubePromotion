// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCGameMode.h"

APCGameMode::APCGameMode()
{
	// 에셋의 클래스 정보를 가져오기 위해서 ConstructorHelpers를 사용함
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/PandoraCube.PCCharacterPlayer"));
	if (DefaultPawnClassRef.Class)
	{
		// 게임모드의 기본 폰을 ABCharacterPlayer로 변경함
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	// 기본적으로 클래스 정보이기 때문에 _C를 붙이지 않음
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/PandoraCube.PCPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		// 게임모드의 플레이어 컨트롤러를 AABPlayerController로 변경함
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
