// Fill out your copyright notice in the Description page of Project Settings.


#include "SPotionPowerUp.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"

ASPotionPowerUp::ASPotionPowerUp()
{
	Power = 40;
	CreditCost = 10;
}

void ASPotionPowerUp::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	if(!InstigatorPawn) return;

	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(InstigatorPawn);

	if(AttributeComponent && !AttributeComponent->IsFullHealth())
	{
		if(ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
			if(PS->RemoveCredits(CreditCost) && AttributeComponent->ApplyHealthChange(InstigatorPawn, Power))
				HidePowerUp();

	}

		
}
