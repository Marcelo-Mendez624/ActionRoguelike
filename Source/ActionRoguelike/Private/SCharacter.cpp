// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "SInteractionComponent.h"
#include "SMagicProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	ArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("ArmComponent"));
	ArmComponent->SetupAttachment(RootComponent);
	ArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(ArmComponent);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComp"));

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Get the player controller
	APlayerController* PC = Cast<APlayerController>(GetController());

	// Get the local player subsystem
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	// Clear out existing mapping, and add our mapping
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);
}



// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* IC = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	IC->BindAction(InputMove, ETriggerEvent::Triggered, this, &ASCharacter::Move);
	
	IC->BindAction(InputJump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	
	IC->BindAction(InputLook, ETriggerEvent::Triggered, this, &ASCharacter::Look);
	
	IC->BindAction(InputAttack, ETriggerEvent::Started, this, &ASCharacter::PrimaryAttack);
	
	IC->BindAction(InputInteraction, ETriggerEvent::Started, this, &ASCharacter::Interact);
	
}

void ASCharacter::Move(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const FVector2D MoveValue = Value.Get<FVector2D>();
		const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);
 
		// Forward/Backward direction
		if (MoveValue.Y != 0.f)
		{
			// Get forward vector
			const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
 
			AddMovementInput(Direction, MoveValue.Y);
		}
 
		// Right/Left direction
		if (MoveValue.X != 0.f)
		{
			// Get right vector
			const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);
 
			AddMovementInput(Direction, MoveValue.X);
		}
	}
}
 
void ASCharacter::Look(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const FVector2D LookValue = Value.Get<FVector2D>();
 
		if (LookValue.X != 0.f)
			AddControllerYawInput(LookValue.X);
		
 
		if (LookValue.Y != 0.f)
			AddControllerPitchInput(LookValue.Y);
		
	}
}

void ASCharacter::PrimaryAttack(const FInputActionValue& Value)
{

	if(AttackAnim)
		PlayAnimMontage(AttackAnim);
	
	GetWorldTimerManager().SetTimer(HandleAttack, this, &ASCharacter::PrimaryAttack_TimerElapsed, .2f);
}

void ASCharacter::PrimaryAttack_TimerElapsed()
{
	const FVector HandLocation = GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));
	
	const FTransform SpawnTM = FTransform(GetActorRotation(),HandLocation);

	FActorSpawnParameters SpawnParameters;

	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = this;
	
	if(ProjectileClass)
		GetWorld()->SpawnActor<ASMagicProjectile>(ProjectileClass, SpawnTM, SpawnParameters);
}

void ASCharacter::Interact(const FInputActionValue& Value)
{
	InteractionComp->PrimaryInteract();
}
