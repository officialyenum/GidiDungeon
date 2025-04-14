// Copyright Chukwuyenum Opone - yenum.dev 2025


#include "Component/GD_WeaponProjectileComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/GD_Character.h"
#include "GameFramework/Character.h"
#include "Projectile/GD_ProjectileBase.h"

UGD_WeaponProjectileComponent::UGD_WeaponProjectileComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	
	ProjectileClass = AGD_ProjectileBase::StaticClass();
}

void UGD_WeaponProjectileComponent::BeginPlay()
{
	Super::BeginPlay();

	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;
	if (const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(WeaponMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Triggered, this, &UGD_WeaponProjectileComponent::Throw);
		}
	}
	
}

void UGD_WeaponProjectileComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void UGD_WeaponProjectileComponent::SetProjectileClass(TSubclassOf<AGD_ProjectileBase> NewProjectileClass)
{
	ProjectileClass = NewProjectileClass;
}

void UGD_WeaponProjectileComponent::Throw(const FInputActionValue& Value)
{
	Throw_Server();
}

void UGD_WeaponProjectileComponent::Throw_Server_Implementation()
{
	if (ProjectileClass)
	{
		const auto Character = Cast<AGD_Character>(GetOwner());
		const auto ProjectileSpawnLocation = GetComponentLocation();
		const auto ProjectileSpawnRotation = GetComponentRotation();
		auto ProjectileSpawnParams = FActorSpawnParameters();
		ProjectileSpawnParams.Owner = GetOwner();
		ProjectileSpawnParams.Instigator = Character;

		GetWorld()->SpawnActor<AGD_ProjectileBase>(ProjectileClass, ProjectileSpawnLocation, ProjectileSpawnRotation, ProjectileSpawnParams);
	}
}
