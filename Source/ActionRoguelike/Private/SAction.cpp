// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

#include "SActionComponent.h"
#include "Net/UnrealNetwork.h"

USAction::USAction()
{
	RepData.bIsRunning = false;
}


bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if(IsRunning()) return false;
	
	USActionComponent* Comp = GetOwningComponent();

	if(Comp->ActiveGameplayTags.HasAny(BlockTags)) return false;

	return true;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	
	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if(GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
		TimeStarted = GetWorld()->TimeSeconds;
	
	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	
	GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}

bool USAction::IsRunning() const
{
	return RepData.bIsRunning;
}


void USAction::OnRep_RepData()
{
	if(RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}


void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, TimeStarted);
}