// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;
	RageMax = 100;
	Rage = 0;

	SetIsReplicatedByDefault(true);

	// ...
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if(ensure(FromActor))
	{
		USAttributeComponent* Att = Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		return Att;
	}
	return nullptr;
}

bool USAttributeComponent::Kill(AActor* Instigator)
{
	return ApplyHealthChange(Instigator, -HealthMax);
	
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComponent = GetAttributes(Actor);
	if(ensure(AttributeComponent)) return AttributeComponent->IsAlive();
	
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
	if(!GetOwner()->CanBeDamaged()) return false;

	if(Delta < 0 && InstigatorActor == GetOwner()) return false;
	
	const float OldHealth = Health;
	const float NewHealth =FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	const float ActualDelta = NewHealth - OldHealth;
	
	if(GetOwner()->HasAuthority())
	{
		Health = NewHealth;
		
		if(ActualDelta != 0.f)
			MulticastHealthChanged(InstigatorActor, Health, Delta);
		
		if(ActualDelta < 0.0f && Health == 0.f)
		{
			ASGameModeBase* GM = Cast<ASGameModeBase>(GetWorld()->GetAuthGameMode<ASGameModeBase>());
			if(GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}

	return ActualDelta != 0;
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME_CONDITION(USAttributeComponent, HealthMax, COND_InitialOnly);
}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}