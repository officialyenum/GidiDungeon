// Copyright Chukwuyenum Opone - yenum.dev 2025


#include "PickUp/GD_BasePickup.h"

#include "Character/GD_Character.h"
#include "Components/SphereComponent.h"


// Sets default values
AGD_BasePickup::AGD_BasePickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	RootComponent = SphereCollision;
	SphereCollision->SetGenerateOverlapEvents(true);
	SphereCollision->SetSphereRadius(200.f);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SphereCollision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AGD_BasePickup::BeginPlay()
{
	Super::BeginPlay();
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AGD_BasePickup::OnBeginOverlap);
}

void AGD_BasePickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const auto Character = Cast<AGD_Character>(OtherActor))
	{
		Pickup(Character);
	}
}

void AGD_BasePickup::Pickup_Implementation(AGD_Character* OwningCharacter)
{
	SetOwner(OwningCharacter);
}

// Called every frame
void AGD_BasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

