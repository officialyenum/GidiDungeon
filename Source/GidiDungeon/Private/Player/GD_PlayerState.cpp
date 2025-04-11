// Copyright Chukwuyenum Opone - yenum.dev 2025


#include "Player/GD_PlayerState.h"

#include "Character/GD_Character.h"
#include "Data/Structs/CharacterStats.h"
#include "Net/UnrealNetwork.h"

void AGD_PlayerState::OnRep_XP(int32 OldValue) const
{
	OnXpChanged.Broadcast(Xp);
}

void AGD_PlayerState::OnRep_CharacterLevelUp(int32 OldValue) const
{
	OnCharacterLevelUp.Broadcast(CharacterLevel);
}

void AGD_PlayerState::AddXp(int32 Value)
{
	Xp += Value;
	OnXpChanged.Broadcast(Xp);

	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, FString::Printf(TEXT("Total XP : %d"), Value));
	if (const auto Character = Cast<AGD_Character>(GetPawn()))
	{
		if (Character->GetCharacterStats()->NextLevelXP < Xp)
		{
			GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, TEXT("Level Up"));
			CharacterLevel++;
			Character->UpdateCharacterStats(CharacterLevel);
			OnCharacterLevelUp.Broadcast(CharacterLevel);
	
			
		}
	}
}

void AGD_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AGD_PlayerState, Xp, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AGD_PlayerState, CharacterLevel, COND_OwnerOnly);
}
