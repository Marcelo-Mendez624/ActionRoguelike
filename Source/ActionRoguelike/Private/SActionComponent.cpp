// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"

#include "SAction.h"
#include "ActionRoguelike/ActionRoguelike.h"

// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for(USAction* Action: Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		FString ActionMsg = FString::Printf(TEXT("[%s] Action %s : IsRunning: %s : Outer: %s"),
			*GetNameSafe(GetOwner()),
			*Action->ActionName.ToString(),
			Action->IsRunning() ? TEXT("True") : TEXT("False"),
			*GetNameSafe(GetOuter()));

		LogOnScreen(this, ActionMsg, TextColor, 0.0);
	}
	
}


// Called when the game starts
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for(TSubclassOf<USAction> ActionsClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionsClass);
	}
	
}

void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if(!ActionClass) return;
	
	USAction* NewAction = NewObject<USAction>(this, ActionClass);

	if(!NewAction) return;
	
	Actions.Add(NewAction);

	if(NewAction->bAutoStart && NewAction->CanStart(Instigator))
		NewAction->StartAction(Instigator);
	
}

void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if (ActionToRemove && !ActionToRemove->IsRunning())
		Actions.Remove(ActionToRemove);
	
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for(USAction* Action : Actions)
	{
		if(Action && Action->ActionName == ActionName)
		{
			if(!Action->CanStart(Instigator)) continue;

			if(!GetOwner()->HasAuthority()) // Is client?
				ServerStartAction(Instigator, ActionName);
			
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for(USAction* Action : Actions)
	{
		if(Action && Action->ActionName == ActionName)
		{
			if( !Action->IsRunning() ) return false;
				
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}

