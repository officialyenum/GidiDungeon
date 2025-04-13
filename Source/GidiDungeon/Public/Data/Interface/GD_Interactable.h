// Copyright Chukwuyenum Opone - yenum.dev 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GD_Interactable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UGD_Interactable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GIDIDUNGEON_API IGD_Interactable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction", meta=(DisplayName="Interact"))
	void Interact(class AGD_Character* CharacterInstigator);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction", meta=(DisplayName="Can Interact"))
	bool CanInteract(class AGD_Character* CharacterInstigator) const;
};
