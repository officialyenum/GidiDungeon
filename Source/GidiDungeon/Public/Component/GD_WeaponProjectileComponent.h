// Copyright Chukwuyenum Opone - yenum.dev 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GD_WeaponProjectileComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GIDIDUNGEON_API UGD_WeaponProjectileComponent : public USceneComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Components", meta=(AllowPrivateAccess = "true"))
	TSubclassOf<class AGD_ProjectileBase> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile|Components|Input", meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* WeaponMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile|Components|Input", meta=(AllowPrivateAccess = "true"))
	class UInputAction* ThrowAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Animations", meta=(AllowPrivateAccess = "true"))
	UAnimMontage* ThrowAnimation;
public:
	// Sets default values for this component's properties
	UGD_WeaponProjectileComponent();

	UFUNCTION(BlueprintCallable, Category = "Projectile|Components|Input")
	void SetProjectileClass(TSubclassOf<class AGD_ProjectileBase> NewProjectileClass);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void Throw(const struct FInputActionValue& Value);

	// Replicated Server Functions
	UFUNCTION(Server, Reliable)
	void Throw_Server();
	// Replicated Client Functions
	UFUNCTION(NetMulticast, Unreliable)
	void Throw_Client();
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
