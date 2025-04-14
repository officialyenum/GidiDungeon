// Copyright Chukwuyenum Opone - yenum.dev 2025


#include "Character/GD_Character.h"

#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Component/GD_WeaponProjectileComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Data/Interface/GD_Interactable.h"
#include "Data/Structs/CharacterStats.h"
#include "Kismet/KismetSystemLibrary.h"


AGD_Character::AGD_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	
	NoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Noise Emitter"));
	NoiseEmitter->NoiseLifetime = 0.01f;

	Weapon = CreateDefaultSubobject<UGD_WeaponProjectileComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(RootComponent);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll= false;
	bUseControllerRotationYaw = false;

	GetCapsuleComponent()->InitCapsuleSize(60.f, 96.f);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
	SkeletalMeshAsset(TEXT("/Game/KayKit/Characters/rogue.rogue"));
	if (SkeletalMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object);
		GetMesh()->SetRelativeLocation(FVector(0.f,0.f,-91.f));
	}
	Weapon->SetRelativeLocation(FVector(120.0f, 70.0f, 0.f));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,500.f,0.f);
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	
}

void AGD_Character::UpdateCharacterStats(int32 CharacterLevel)
{
	auto bIsSprinting = false;
	if (GetCharacterStats())
	{
		bIsSprinting = GetCharacterMovement()->MaxWalkSpeed == GetCharacterStats()->SprintSpeed;
	}
	if(CharacterDataTable)
	{
		TArray<FCharacterStats*> CharacterStatsRows;
		CharacterDataTable->GetAllRows<FCharacterStats>(TEXT("GD Character"), CharacterStatsRows);
		if (CharacterStatsRows.Num() > 0)
		{
			const auto NewCharacterLevel = FMath::Clamp(CharacterLevel, 1, CharacterStatsRows.Num());
			CharacterStats = CharacterStatsRows[NewCharacterLevel - 1];
			GetCharacterMovement()->MaxWalkSpeed = GetCharacterStats()->WalkSpeed;
			if (bIsSprinting)
			{
				SprintStart_Server();
			}
		}
	}
}

void AGD_Character::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	UpdateCharacterStats(1);
	
}

void AGD_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() != ROLE_Authority) return;
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(this);
	auto SphereRadius = 50.f;

	auto StartLocation = GetActorLocation() + GetActorForwardVector() * 150.f;
	auto EndLocation = StartLocation + GetActorForwardVector() * 500.f;

	auto bIsHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		StartLocation,
		EndLocation,
		SphereRadius,
		UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
		false
		, TArray<AActor*>(),
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
		);

	if (bIsHit && HitResult.GetActor()->GetClass()->ImplementsInterface(UGD_Interactable::StaticClass()))
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, SphereRadius, 12, FColor::Magenta, false, 1.f);
		InteractableActor = HitResult.GetActor();
	}
	else
	{
		InteractableActor = nullptr;
	}

	if (GetCharacterMovement()->MaxWalkSpeed == GetCharacterStats()->SprintSpeed)
	{
			auto Noise = 1.f;
			if (GetCharacterStats() && GetCharacterStats()->StealthMultiplier)
			{
				Noise = Noise / GetCharacterStats()->StealthMultiplier;
			}
			NoiseEmitter->MakeNoise(this, Noise, GetActorLocation());
	}
	
}

void AGD_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGD_Character::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGD_Character::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AGD_Character::Interact);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AGD_Character::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AGD_Character::SprintEnd);
	}
}

void AGD_Character::Move(const FInputActionValue& Value)
{
	const auto MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Yellow, FString::Printf(TEXT("Movement Vector : %s"), *MovementVector.ToString()));
		const auto Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const auto ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const auto RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGD_Character::Look(const FInputActionValue& Value)
{
	const auto LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Look Axis : %s"), *LookAxisVector.ToString()));
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGD_Character::SprintStart(const FInputActionValue& Value)
{
	SprintStart_Server();
}

void AGD_Character::SprintEnd(const FInputActionValue& Value)
{
	SprintEnd_Server();
}

void AGD_Character::Interact(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, TEXT("Interact"));
	Interact_Server();
}

void AGD_Character::SprintStart_Server_Implementation()
{
	if (GetCharacterStats())
	{
		GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Blue, TEXT("StartSprint"));
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterStats()->SprintSpeed;
	}
}

void AGD_Character::SprintEnd_Server_Implementation()
{
	if (GetCharacterStats())
	{
		GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Blue, TEXT("EndSprint"));
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterStats()->WalkSpeed;
	}
}

void AGD_Character::Interact_Server_Implementation()
{
	if (InteractableActor)
	{
		IGD_Interactable::Execute_Interact(InteractableActor, this);
	}
}
