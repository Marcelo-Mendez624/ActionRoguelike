// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

/**
 * 
 */

class ASProjectileBase;
class UAnimMontage;
class UParticleSystem;

UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

public:
	USAction_ProjectileAttack();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ASProjectileBase> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	FName HandSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UAnimMontage* AttackAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UParticleSystem* CastingEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);
	
};
