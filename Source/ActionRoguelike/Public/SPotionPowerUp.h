// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBasePowerUp.h"
#include "SPotionPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPotionPowerUp : public ASBasePowerUp
{
	GENERATED_BODY()

public:
	ASPotionPowerUp();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;


protected:

	UPROPERTY(EditDefaultsOnly, Category = "Interact")
	float Power;
	
	UPROPERTY(EditDefaultsOnly, Category = "Interact")
	float CreditCost;
};
