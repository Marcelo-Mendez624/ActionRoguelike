// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

/**
 * 
 */

class USActionComponent;
class UWorld;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

public:
	USAction();
	
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);
	
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintNativeEvent, Category ="Action")
	void StartAction(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent, Category ="Action")
	void StopAction(AActor* Instigator);
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool IsRunning() const;


protected:
	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;

	UFUNCTION()
	void OnRep_RepData();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* Icon;
	

	UPROPERTY(Replicated)
	float TimeStarted;
	
	UPROPERTY(EditDefaultsOnly, Category ="Tags")
	FGameplayTagContainer GrantsTags;
	
	UPROPERTY(EditDefaultsOnly, Category ="Tags")
	FGameplayTagContainer BlockTags;

	UFUNCTION(BlueprintCallable, Category = "Action")
	USActionComponent* GetOwningComponent() const;

	virtual bool IsSupportedForNetworking() const override { return true; }
};
