// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

#include "SActionComponent.h"

USAction::USAction()
{
	bIsRunning = false;
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

	bIsRunning = true;
	
	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantsTags);
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);
	
	bIsRunning = false;
	
	GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantsTags);
}



UWorld* USAction::GetWorld() const
{
	return UObject::GetWorld();
}

bool USAction::IsRunning() const
{
	return bIsRunning;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}
