// Copyright Chukwuyenum Opone - yenum.dev 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GD_MinionSpawner.generated.h"

UCLASS()
class GIDIDUNGEON_API AGD_MinionSpawner : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "System|Spawner", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> SpawnArea;


	UPROPERTY()
	FTimerHandle SpawnTimerHandle;
public:
	// Sets default values for this actor's properties
	AGD_MinionSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "System|Spawner")
	TArray<TSubclassOf<class AGD_Minion>> SpawnableMinions;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "System|Spawner")
	float SpawnDelay{ 10.0f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "System|Spawner")
	int32 NumMinionsAtStart{ 5 };
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void Spawn();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
