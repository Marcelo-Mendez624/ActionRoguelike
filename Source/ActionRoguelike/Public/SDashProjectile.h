// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()

public:
	ASDashProjectile();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;
	float DetonateDelay;

	virtual void Explode_Implementation() override;

	FTimerHandle TimerHandle_DelayDetonate;
	

	void TeleportInstigator() const;
	
};
