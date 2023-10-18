// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "SWorldUserWidget.h"
#include "Blueprint/UserWidget.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TraceDistance = 500.f;
	TraceRadius = 20.f;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USInteractionComponent::FindBestInteractable()
{
	FHitResult HitResult;
	
	FVector Start;
	FRotator EyeRotation;
	
	GetOwner()->GetActorEyesViewPoint(Start, EyeRotation);
	
	const FVector End =  Start + ( EyeRotation.Vector() * TraceDistance );
	
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	TArray<FHitResult> Hits;

	float Radius = TraceRadius;
	
	
	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	
	
	bool bBlockHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, QueryParams, Shape);
	
	FocusedActor = nullptr;
	
	for(FHitResult Hit : Hits)
	{
		if(AActor* HitActor = Hit.GetActor())
		{
			if(HitActor->Implements<USGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if(FocusedActor)
	{
		if(DefaultWidgetInstance == nullptr && DefaultWidgetClass)
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);

		if(DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachActor = FocusedActor;
			if(!DefaultWidgetInstance->IsInViewport())
				DefaultWidgetInstance->AddToViewport();
		}
	}
	else
	{
		if(DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = Cast<APawn>(GetOwner());
	if(MyPawn->IsLocallyControlled())
		FindBestInteractable();
}

void USInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if(!InFocus) return;
	
	APawn* MyPawn = Cast<APawn>(GetOwner());
			
	ISGameplayInterface::Execute_Interact(InFocus, MyPawn);
}

