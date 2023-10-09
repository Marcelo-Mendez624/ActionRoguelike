// Fill out your copyright notice in the Description page of Project Settings.


#include "SPotionPowerUp.h"

#include "SAttributeComponent.h"

ASPotionPowerUp::ASPotionPowerUp()
{
	Power = 10;
}

void ASPotionPowerUp::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	if(!InstigatorPawn) return;

	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(InstigatorPawn);

	if(AttributeComponent && !AttributeComponent->IsFullHealth())
	{
		if(AttributeComponent->ApplyHealthChange(InstigatorPawn, Power))
			HidePowerUp();
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Faild Cure"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Faild Attribute"));
	}
		
}
