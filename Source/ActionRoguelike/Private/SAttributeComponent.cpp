// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"


// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;

	// ...
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if(FromActor)
	{
		USAttributeComponent* Att = Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		return Att;
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComponent = GetAttributes(Actor);
	if(AttributeComponent)
	{
		return AttributeComponent->IsAlive();
	}

	return false;
}


bool USAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	const float OldHealth = Health;
	Health += Delta;

	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
	
	return ActualDelta != 0;
}

