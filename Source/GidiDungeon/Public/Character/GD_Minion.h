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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Minion|Health", meta=(AllowPrivateAccess = "true"))
	float Health{ 5.0f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Minion|Pickup", meta=(AllowPrivateAccess = "true"))
	TSubclassOf<class AGD_BasePickup> SpawnedPickUp;
public:
	AGD_Minion();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Minion|Movement")
	float PatrolSpeed{ 150.0f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Minion|Movement")
	float ChaseSpeed{ 350.0f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Minion|Movement")
	float PatrolRadius{ 50000.0f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Minion|AI")
	float AlertRadius{ 6000.0f };
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
public:
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Minion|AI")
	void SetNextPatrolLocation();
	UFUNCTION(BlueprintCallable, Category = "Minion|AI")
	void Chase(APawn* Pawn);
	UFUNCTION(BlueprintCallable, Category = "Minion|AI")
	void GoToLocation(const FVector& Location);

	virtual void PostInitializeComponents() override;


	UFUNCTION()
	void OnPawnDetected(APawn* Pawn);
	UFUNCTION()
	void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);
	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	
	FORCEINLINE UPawnSensingComponent* GetPawnSense() const { return PawnSense; }
	FORCEINLINE USphereComponent* GetCollision() const { return Collision; }
};
