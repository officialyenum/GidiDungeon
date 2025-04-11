// Copyright Chukwuyenum Opone - yenum.dev 2025


#include "Game/GD_GameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Game/GD_GameState.h"
#include "Player/GD_PlayerController.h"
#include "Player/GD_PlayerState.h"

AGD_GameMode::AGD_GameMode()
{
	GameStateClass = AGD_GameState::StaticClass();
	PlayerStateClass = AGD_PlayerState::StaticClass();
	PlayerControllerClass = AGD_PlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn>
	PlayerPawnBPClass(TEXT("/Game/Blueprints/Character/BP_Character"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
