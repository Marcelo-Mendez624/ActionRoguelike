// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

/**
 * 
 */

class UEnvQueryInstanceBlueprintWrapper;

UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:

	ASGameModeBase();
	
	virtual void StartPlay() override;

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	
protected:
	UFUNCTION(Exec)
	void CallBots();
	UFUNCTION(Exec)
	void StopBots();
	FTimerHandle TimerHandle_SpawnBots;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UCurveFloat* DifficultyCurve;
	
	UFUNCTION()
	void SpawnBotTimerElapsed();

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UEnvQuery* SpawnBotQuery;

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	UPROPERTY(EditDefaultsOnly, Category = "Respawn")
	float RespawnDelay;
	UPROPERTY(EditDefaultsOnly, Category = "Kill")
	float CreditsPerKill;

	

};
