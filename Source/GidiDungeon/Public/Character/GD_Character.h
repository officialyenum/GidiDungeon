// Copyright Chukwuyenum Opone - yenum.dev 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GD_Character.generated.h"

UCLASS()
class GIDIDUNGEON_API AGD_Character : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom|Camera", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom|Camera", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom|Input", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom|Input", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom|Input", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom|Input", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SprintAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom|Input", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractAction;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom|Data", meta=(AllowPrivateAccess = "true"))
	class UDataTable* CharacterDataTable;

	struct FCharacterStats* CharacterStats;
public:
	// Sets default values for this character's properties
	AGD_Character();

	void UpdateCharacterStats(int32 CharacterLevel);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);
	void SprintStart(const struct FInputActionValue& Value);
	void SprintEnd(const struct FInputActionValue& Value);
	void Interact(const struct FInputActionValue& Value);

	// Replicated Server Functions
	UFUNCTION(Server, Reliable)
	void SprintStart_Server();
	UFUNCTION(Server, Reliable)
	void SprintEnd_Server();
	UFUNCTION(Server, Reliable)
	void Interact_Server();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE FCharacterStats* GetCharacterStats() const { return CharacterStats; }
private:
	UPROPERTY()
	AActor* InteractableActor;
};
