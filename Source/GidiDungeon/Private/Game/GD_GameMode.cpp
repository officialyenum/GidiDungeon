// Copyright Chukwuyenum Opone - yenum.dev 2025


#include "Game/GD_GameMode.h"

#include "Character/GD_Minion.h"
#include "UObject/ConstructorHelpers.h"
#include "Game/GD_GameState.h"
#include "Kismet/GameplayStatics.h"
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

void AGD_GameMode::AlertMinions(AActor* AlertInstigator, const FVector& Location, float Radius)
{
	TArray<AActor*> Minions;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGD_Minion::StaticClass(), Minions);
	for (const auto Minion: Minions)
	{
		if (AlertInstigator == Minion) continue;
		if (const auto Distance = FVector::Distance(AlertInstigator->GetActorLocation(), Minion->GetActorLocation()); Distance < Radius)
		{
			if (const auto MinionCharacter = Cast<AGD_Minion>(Minion))
			{
				MinionCharacter->GoToLocation(Location);
			}
		}
		
	}
}
