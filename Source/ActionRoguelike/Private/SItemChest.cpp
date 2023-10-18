// Fill out your copyright notice in the Description page of Project Settings.
#include "SItemChest.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	SetReplicates(true);
	TargetPitch = 110;
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpen = !bLidOpen;
	OnRep_LidOpened();
}

void ASItemChest::OnRep_LidOpened()
{
	const float CurrentPitch = bLidOpen ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrentPitch, 0, 0));
}

void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASItemChest, bLidOpen);
	
}