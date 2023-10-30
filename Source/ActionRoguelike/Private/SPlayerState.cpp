// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "SSaveGame.h"
#include "Net/UnrealNetwork.h"

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

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Credits);
}

void ASPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits,Credits-OldCredits);
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{
	if(SaveObject)
	{
		AddCredits(SaveObject->Credits);
	}
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject)
{
	if(SaveObject)
	{
		SaveObject->Credits = Credits;
	}
}
