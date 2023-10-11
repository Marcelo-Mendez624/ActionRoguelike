// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DamageAmount = 20;
	bReflected = false;
	
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnComponentOverlap);
	
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASMagicProjectile::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	USActionComponent* ActionComponent = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));

	if(ActionComponent && !bReflected && ActionComponent->ActiveGameplayTags.HasTag(ParryTag))
	{
		bReflected = true;
		MovementComponent->Velocity = -MovementComponent->Velocity;
		SetInstigator(Cast<APawn>(OtherActor));
		return;
	}

	if(USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
	{
		Explode();
	}
}

