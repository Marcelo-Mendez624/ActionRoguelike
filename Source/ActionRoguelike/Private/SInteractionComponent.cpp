// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	FHitResult HitResult;
	
	FVector Start;
	FRotator EyeRotation;
	
	GetOwner()->GetActorEyesViewPoint(Start, EyeRotation);
	
	const FVector End =  Start + ( EyeRotation.Vector() * 500 );
	
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	TArray<FHitResult> Hits;

	float Radius = 30;
	
	
	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	
	
	bool bBlockHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, QueryParams, Shape);

	FColor LineColor = bBlockHit ? FColor::Green : FColor::Red;
	
	for(FHitResult Hit : Hits)
	{
		if(AActor* HitActor = Hit.GetActor())
		{
			if(HitActor->Implements<USGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(GetOwner());
			
				ISGameplayInterface::Execute_Interact(HitActor, MyPawn);

				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, true,2.f);
				break;
			}
		}
	}
	
	DrawDebugLine(GetWorld(), Start, End, LineColor, true, 2);

	
}

