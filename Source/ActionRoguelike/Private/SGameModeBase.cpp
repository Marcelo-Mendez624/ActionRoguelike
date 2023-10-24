// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "SSaveGame.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/GameplayStatics.h"


ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.f;
	RespawnDelay = 2.f;
	CreditsPerKill = 50;
	PlayerStateClass = ASPlayerState::StaticClass();
	SlotName = "SaveGame01";
}


void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if(Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());
		
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	APawn* KillerPawn = Cast<APawn>(Killer);
	if(Killer)
	{
		ASPlayerState* PS = KillerPawn->GetPlayerState<ASPlayerState>();
		if(PS)
			PS->AddCredits(CreditsPerKill);
	}
}

void ASGameModeBase::WriteSaveGame()
{
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ASGameModeBase::LoadSaveGame()
{
	if(UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
		if(CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Faild to load save game"));
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("Load Game"));
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
		UE_LOG(LogTemp, Warning, TEXT("Created New SaveGame"));
	}
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
	
}

void ASGameModeBase::CallBots()
{
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::StopBots()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_SpawnBots);
}

void ASGameModeBase::SpawnBotTimerElapsed()
{

	int32 NrOfAliveBots = 0;
	for(TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(Bot);
		
		if(AttributeComponent && AttributeComponent->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	float MaxBotCount = 10;

	if(DifficultyCurve)
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	
	
	if(NrOfAliveBots >= MaxBotCount) return;
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	
	
	
	if(QueryInstance)
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Spawn bot EQS Query Faild!"));
		return;
	}
	
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if(Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<ASAICharacter>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if(Controller)
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}
