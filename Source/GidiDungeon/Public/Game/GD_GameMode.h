// Copyright Chukwuyenum Opone - yenum.dev 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GD_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class GIDIDUNGEON_API AGD_GameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AGD_GameMode();

	UFUNCTION(BlueprintCallable, Category="GM|Minions")
	void AlertMinions(class AActor* AlertInstigator, const FVector& Location, float Radius);
};
