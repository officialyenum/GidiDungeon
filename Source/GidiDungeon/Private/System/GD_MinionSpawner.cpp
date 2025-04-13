// Copyright Chukwuyenum Opone - yenum.dev 2025


#include "System/GD_MinionSpawner.h"

#include "Character/GD_Minion.h"
#include "Components/BoxComponent.h"


// Sets default values
AGD_MinionSpawner::AGD_MinionSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn Area"));
	SpawnArea->SetupAttachment(RootComponent);
	SpawnArea->SetBoxExtent(FVector(1000.0f, 1000.0f, 100.0f));

	
}

// Called when the game starts or when spawned
void AGD_MinionSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (SpawnableMinions.IsEmpty()) return;
	if (GetLocalRole() != ROLE_Authority) return;
	for (int32 i = 0; i < NumMinionsAtStart; i++)
	{
		Spawn();
	}
	
GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AGD_MinionSpawner::Spawn, SpawnDelay,true, SpawnDelay);	
}

void AGD_MinionSpawner::Spawn()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	auto minion = SpawnableMinions[FMath::RandRange(0, SpawnableMinions.Num() - 1)];
	const auto Rotation = FRotator(0.0f, FMath::RandRange(0.f, 360.0f), 0.f);
	const auto Location = SpawnArea->GetComponentLocation() + FVector(
		FMath::RandRange(-SpawnArea->GetScaledBoxExtent().X, SpawnArea->GetScaledBoxExtent().X),
		FMath::RandRange(-SpawnArea->GetScaledBoxExtent().Y, SpawnArea->GetScaledBoxExtent().Y),
		0.f);

	GetWorld()->SpawnActor<AGD_Minion>(minion, Location, Rotation, SpawnParams);
}

// Called every frame
void AGD_MinionSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

