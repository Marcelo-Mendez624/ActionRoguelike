// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Public\SBasePowerUp.h"

#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ASBasePowerUp::ASBasePowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	TimeToReactivate = 10.f;
	
}

// Called when the game starts or when spawned
void ASBasePowerUp::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void ASBasePowerUp::Inactivate()
{
	MeshComponent->SetVisibility(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASBasePowerUp::Activate()
{
	MeshComponent->SetVisibility(true);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

// Called every frame
void ASBasePowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASBasePowerUp::Interact_Implementation(APawn* InstigatorPawn)
{
	ISGameplayInterface::Interact_Implementation(InstigatorPawn);

	Inactivate();

	FTimerHandle HandleActivation;
	GetWorldTimerManager().SetTimer(HandleActivation, this, &ASBasePowerUp::Activate, TimeToReactivate);
	
}

