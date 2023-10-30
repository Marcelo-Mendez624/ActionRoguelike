// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, Instigator, USAttributeComponent*, OwningComp, float, NewValue, float, Delta);

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float HealthMax;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated,Category = "Attributes")
	float RageMax;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated,Category = "Attributes")
	float Rage;

	UFUNCTION(NetMulticast, Reliable) // @Fixme: mark as unreliable once we moved the 'state' out of SCharacter
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);
	
	UFUNCTION(NetMulticast, Unreliable) // @Fixme: mark as unreliable once we moved the 'state' out of SCharacter
	void MulticastRageChanged(AActor* InstigatorActor, float NewRage, float Delta);

public:	

	UFUNCTION(BlueprintCallable)
    bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	bool IsFullHealth() const;
    
	UFUNCTION(Blueprintable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);
	

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;
		
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRangeChanged;		
};
