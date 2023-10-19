// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Public\SBasePowerUp.h"

#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ASBasePowerUp::ASBasePowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionProfileName("PowerUp");
	RootComponent = SphereComponent;
	
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(SphereComponent);

	TimeToReactivate = 10.f;

	SetReplicates(true);
}


void ASBasePowerUp::SetPowerUpState(bool bIsActive)
{
	SetActorEnableCollision(bIsActive);

	RootComponent->SetVisibility(bIsActive, true);
}

void ASBasePowerUp::ActivePowerUp()
{
	SetPowerUpState(true);
}

void ASBasePowerUp::HidePowerUp()
{
	SetPowerUpState(false);

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASBasePowerUp::ActivePowerUp, TimeToReactivate);
}

void ASBasePowerUp::Interact_Implementation(APawn* InstigatorPawn)
{
	ISGameplayInterface::Interact_Implementation(InstigatorPawn);
	
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, TEXT("Interact"));
}

