// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SActionEffect.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	DamageAmount = 20;
	bReflected = false;
	
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnComponentOverlap);
	
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
		if(ActionComponent && HasAuthority())
		{
			ActionComponent->AddAction(GetInstigator(), ActionEffect);
		}
	}
}

