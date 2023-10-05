// Fill out your copyright notice in the Description page of Project Settings.


#include "SBlackholeProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASBlackholeProjectile::ASBlackholeProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphreComponent"));
	RootComponent = SphereComponent;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForceComponent->SetupAttachment(SphereComponent);
	RadialForceComponent->ForceStrength = -20000000.f;
	RadialForceComponent->Radius = 1000.f;
	
	
}

// Called when the game starts or when spawned
void ASBlackholeProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASBlackholeProjectile::OnOverlapBegin);

	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, this, &ASBlackholeProjectile::DestroyAfter, 5.f);
}

void ASBlackholeProjectile::DestroyAfter()
{
	Destroy();
}

// Called every frame
void ASBlackholeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ASBlackholeProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherComp)
		if(OtherComp->IsSimulatingPhysics())
			OtherActor->Destroy();
}

