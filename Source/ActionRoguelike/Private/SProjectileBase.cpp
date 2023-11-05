// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"

// Sets default values
ASProjectileBase::ASProjectileBase()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionProfileName("Projectile");
	SphereComponent->SetCanEverAffectNavigation(false);
	RootComponent = SphereComponent;
	
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	ParticleSystemComponent->SetupAttachment(SphereComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoveComp"));
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;
	MovementComponent->ProjectileGravityScale = 0.f;
	MovementComponent->InitialSpeed = 8000;

	// SetReplicates(true);
}



void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
	SphereComponent->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnHit);
}

void ASProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void ASProjectileBase::Explode_Implementation()
{
	if(ensure(!IsPendingKill()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

		Destroy();
	}
}

