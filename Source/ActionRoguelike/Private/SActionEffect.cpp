// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect.h"

#include "BlueprintEditor.h"
#include "SActionComponent.h"

USActionEffect::USActionEffect()
{
	Duration = 1;
	Period = 1;
	bAutoStart = true;
}

void USActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if(Duration > 0)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);
		
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}
	
	if(Period > 0)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
	
}

void USActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if(GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) > KINDA_SMALL_NUMBER)
		ExecutePeriodicEffect_Implementation(Instigator);
	
	Super::StopAction_Implementation(Instigator);
	
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	GetOwningComponent()->RemoveAction(this);
}

void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
}
