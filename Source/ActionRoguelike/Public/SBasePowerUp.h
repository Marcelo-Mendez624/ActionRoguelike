// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SBasePowerUp.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASBasePowerUp : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASBasePowerUp();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly)
	float TimeToReactivate;

	void SetPowerUpState(bool bIsActive);

	void ActivePowerUp();
	void HidePowerUp();

	FTimerHandle RespawnTimerHandle;

public:	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

};
