// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UAnimMontage;
class USAttributeComponent;
class USActionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* ArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComponent;

	UPROPERTY()
	class USInteractionComponent* InteractionComp;

	// Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputMove;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputLook;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputJump;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAttack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputBlackHole;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputInteraction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputSprint;


	// Handle move input
	void Move(const FInputActionValue& Value);
 
	// Handle look input
	void Look(const FInputActionValue& Value);
	
	// Handle Projectile input
	void PrimaryAttack(const FInputActionValue& Value);
	
	// Handle Projectile input
	void BlackHoleAttack(const FInputActionValue& Value);
	
	// Handle SprintStart input
	void SprintStart(const FInputActionValue& Value);
	
	// Handle SprintStop input
	void SprintStop(const FInputActionValue& Value);

	// Handle Interaction input
	void Interact(const FInputActionValue& Value);
	

	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual FVector GetPawnViewLocation() const override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
