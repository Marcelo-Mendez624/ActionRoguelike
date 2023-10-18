// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, Instigator, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged, AActor*, Instigator, USAttributeComponent*, OwningComp, float, NewRage, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

	UFUNCTION(BlueprintCallable, Category = "Attirbute")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* Instigator);
	
	UFUNCTION(Blueprintable, Category = "Attirbute")
	static bool IsActorAlive(AActor* Actor);
	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float HealthMax;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float RageMax;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Rage;

public:	

	UFUNCTION(BlueprintCallable)
    bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	bool IsFullHealth() const;
    
	UFUNCTION(Blueprintable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);
	

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
		
	UPROPERTY(BlueprintAssignable)
	FOnRageChanged OnRangeChanged;		
};
