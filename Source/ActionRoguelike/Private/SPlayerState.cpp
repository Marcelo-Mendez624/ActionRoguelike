// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

void ASPlayerState::AddCredits(int32 Delta)
{
	Credits += Delta;

	OnCreditsChanged.Broadcast(this, GetCredits(), Delta);
}

bool ASPlayerState::RemoveCredits(int32 Delta)
{
	if(Delta < GetCredits())
	{
		Credits -= Delta;
		OnCreditsChanged.Broadcast(this, GetCredits(), Delta);
		return true;
	}

	return false;
}
