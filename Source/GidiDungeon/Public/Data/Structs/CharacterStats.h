// Copyright Chukwuyenum Opone - yenum.dev 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterStats.generated.h"

/**
 * 
 */
USTRUCT()
struct GIDIDUNGEON_API FCharacterStats : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WalkSpeed = 200.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SprintSpeed = 400.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DamageMultiplier = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StealthMultiplier = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 NextLevelXP = 10.0f;
};
