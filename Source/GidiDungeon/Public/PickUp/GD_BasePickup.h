// Copyright Chukwuyenum Opone - yenum.dev 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GD_BasePickup.generated.h"

UCLASS()
class GIDIDUNGEON_API AGD_BasePickup : public AActor
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom|Pickup", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom|Pickup", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Mesh;
public:
	// Sets default values for this actor's properties
	AGD_BasePickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Custom|Pickup", meta=(DisplayName="Pickup"))
	void Pickup(class AGD_Character* OwningCharacter);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	FORCEINLINE USphereComponent* GetSphereCollision() const { return SphereCollision; }
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }
};
