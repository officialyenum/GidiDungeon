// Copyright Chukwuyenum Opone - yenum.dev 2025


#include "Character/GD_Minion.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/GD_Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Game/GD_GameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "PickUp/GD_BasePickup.h"


AGD_Minion::AGD_Minion()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIController::StaticClass();

	PawnSense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sense"));
	PawnSense->SensingInterval = 0.8f;
	PawnSense->SetPeripheralVisionAngle(45.f);
	PawnSense->SightRadius = 1500.f;
	PawnSense->HearingThreshold = 400.f;
	PawnSense->LOSHearingThreshold = 800.f;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetSphereRadius(100.f);
	Collision->SetupAttachment(RootComponent);


	GetCapsuleComponent()->InitCapsuleSize(60.0f, 96.0f);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	GetMesh()->SetRelativeLocation(FVector(0.f,0.f,-91.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
	SkeletalMeshAsset(TEXT("/Game/KayKit/Skeletons/skeleton_minion.skeleton_minion"));
	if (SkeletalMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object);
	}

	static ConstructorHelpers::FClassFinder<AGD_BasePickup>
	PickUpMeshAsset(TEXT("/Game/Blueprints/Pickups/BP_GoldCoin.BP_GoldCoin"));
	if (PickUpMeshAsset.Succeeded())
	{
		SpawnedPickUp = PickUpMeshAsset.Class;
	}

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,500.f,0.f);
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

void AGD_Minion::BeginPlay()
{
	Super::BeginPlay();
	SetNextPatrolLocation();
	OnTakeAnyDamage.AddDynamic(this, &AGD_Minion::OnDamage);
	
}

void AGD_Minion::OnDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
	AActor* DamageCauser)
{
	Health -= Damage;
	if (Health > 0) return;


	if (SpawnedPickUp)
	{
		GetWorld()->SpawnActor<AGD_BasePickup>(SpawnedPickUp, GetActorLocation(), GetActorRotation());
	}
	Destroy();
}

void AGD_Minion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetLocalRole() != ROLE_Authority) return;
	if (GetMovementComponent()->GetMaxSpeed() == ChaseSpeed) return;
	if ((GetActorLocation() - PatrolLocation).Size() < 500.f)
	{
		SetNextPatrolLocation();
	}
}

void AGD_Minion::SetNextPatrolLocation()
{
	if (GetLocalRole() != ROLE_Authority) return;
	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	if (const auto LocationFound = UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, GetActorLocation(), PatrolLocation, PatrolRadius))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), PatrolLocation);
	}
}

void AGD_Minion::Chase(APawn* Pawn)
{
	if (GetLocalRole() != ROLE_Authority) return;
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), Pawn);
	DrawDebugSphere(GetWorld(),Pawn->GetActorLocation(), 25.f, 12,	FColor::Red, true, 10.f, 0, 2.f);

	if (const auto GM = Cast<AGD_GameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->AlertMinions(this, Pawn->GetActorLocation(), AlertRadius);
	}
}

void AGD_Minion::GoToLocation(const FVector& Location)
{
	PatrolLocation = Location;
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), PatrolLocation);
}

void AGD_Minion::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetLocalRole() != ROLE_Authority) return;
	OnActorBeginOverlap.AddDynamic(this, &AGD_Minion::OnBeginOverlap);
	GetPawnSense()->OnSeePawn.AddDynamic(this, &AGD_Minion::OnPawnDetected);
	GetPawnSense()->OnHearNoise.AddDynamic(this, &AGD_Minion::OnHearNoise);
}

void AGD_Minion::OnPawnDetected(APawn* Pawn)
{
	if (!Pawn->IsA<AGD_Character>()) return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Character Detected "));
	if (GetCharacterMovement()->MaxWalkSpeed != ChaseSpeed)
	{
		Chase(Pawn);
	}
		
	
}

void AGD_Minion::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Noise Heard"));
	GoToLocation(Location);
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), PatrolLocation);
}

void AGD_Minion::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!OtherActor->IsA<AGD_Character>()) return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Character Captured"));
}


