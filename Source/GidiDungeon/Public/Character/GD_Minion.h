// Copyright Chukwuyenum Opone - yenum.dev 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GD_Minion.generated.h"

UCLASS()
class GIDIDUNGEON_API AGD_Minion : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minion|Perception", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UPawnSensingComponent> PawnSense;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minion|Perception", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> Collision;

	UPROPERTY()
	FVector PatrolLocation;
public:
	AGD_Minion();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Minion|AI")
	float PatrolSpeed{ 150.0f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Minion|AI")
	float ChaseSpeed{ 350.0f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Minion|AI")
	float PatrolRadius{ 50000.0f };
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Minion|AI")
	void SetNextPatrolLocation();
	UFUNCTION(BlueprintCallable, Category = "Minion|AI")
	void Chase(APawn* Pawn);

	virtual void PostInitializeComponents() override;


	UFUNCTION()
	void OnPawnDetected(APawn* Pawn);
	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	
	FORCEINLINE UPawnSensingComponent* GetPawnSense() const { return PawnSense; }
	FORCEINLINE USphereComponent* GetCollision() const { return Collision; }
};
