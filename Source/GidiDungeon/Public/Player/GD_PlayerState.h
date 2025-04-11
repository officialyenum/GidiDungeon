// Copyright Chukwuyenum Opone - yenum.dev 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GD_PlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnXpChanged, int32, NewXp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterLevelUp, int32, NewLevelXp);
/**
 * 
 */
UCLASS()
class GIDIDUNGEON_API AGD_PlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	// XP
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing="OnRep_Xp", Category="PS|Experience")
	int32 Xp{ 0 };
	UFUNCTION()
	void OnRep_XP(int32 OldValue) const;

	UPROPERTY(BlueprintAssignable, Category="PS|Events")
	FOnXpChanged OnXpChanged;

	// Level Up
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing="OnRep_CharacterLevelUp", Category="PS|Experience")
	int32 CharacterLevel{ 1 };
	UFUNCTION()
	void OnRep_CharacterLevelUp(int32 OldValue) const;

	UPROPERTY(BlueprintAssignable, Category="PS|Events")
	FOnCharacterLevelUp OnCharacterLevelUp;
	
public:
	UFUNCTION(BlueprintCallable, Category="PS|Experience")
	void AddXp(int32 Value);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
