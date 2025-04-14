// Copyright Chukwuyenum Opone - yenum.dev 2025


#include "Projectile/GD_ProjectileBase.h"

#include "Character/GD_Character.h"
#include "Components/SphereComponent.h"
#include "Data/Structs/CharacterStats.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AGD_ProjectileBase::AGD_ProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collission"));
	SphereCollision->SetGenerateOverlapEvents(true);
	SphereCollision->SetSphereRadius(10.0f);
	SphereCollision->BodyInstance.SetCollisionProfileName("BlockAll");
	SphereCollision->OnComponentHit.AddDynamic(this, &AGD_ProjectileBase::OnHit);

	RootComponent = SphereCollision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Mesh->SetRelativeLocation(FVector(-40.f,0.f,0.f));
	Mesh->SetRelativeRotation(FRotator(-90.f,0.f,0.f));
	
	VFXComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("VFXComponent"));
	VFXComponent->SetupAttachment(RootComponent);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
	StaticMeshAsset(TEXT("/Game/KayKit/DungeonElements/dagger_common.dagger_common"));
	if (StaticMeshAsset.Succeeded())
	{
		GetMesh()->SetStaticMesh(StaticMeshAsset.Object);
	}

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = SphereCollision;
	ProjectileMovement->ProjectileGravityScale = 0;
	ProjectileMovement->InitialSpeed = 3000;
	ProjectileMovement->MaxSpeed = 3000;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	bReplicates = true;
}
// Called when the game starts or when spawned
void AGD_ProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGD_ProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	auto ComputedDamage = Damage;
	if (const auto Character =	Cast<AGD_Character>(GetInstigator()))
	{
		ComputedDamage *= Character->GetCharacterStats()->DamageMultiplier;
	}
	if (OtherActor && OtherActor != this)
	{
		const FDamageEvent Event(UDamageType::StaticClass());
		OtherActor->TakeDamage(ComputedDamage, Event, GetInstigatorController(), this);
	}
	Destroy();
}

// Called every frame
void AGD_ProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

