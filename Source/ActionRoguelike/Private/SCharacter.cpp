// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
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
	
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));
	
	ActionComponent = CreateDefaultSubobject<USActionComponent>(TEXT("ActionComponent"));

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
	
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

	const APlayerController* PC = GetController<APlayerController>();
	const ULocalPlayer* LP = PC->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if(Subsystem)
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputMapping, 0);
	}
	
	UEnhancedInputComponent* IC = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	IC->BindAction(InputMove, ETriggerEvent::Triggered, this, &ASCharacter::Move);
	
	IC->BindAction(InputJump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	
	IC->BindAction(InputLook, ETriggerEvent::Triggered, this, &ASCharacter::Look);
	
	IC->BindAction(InputSprint, ETriggerEvent::Started, this, &ASCharacter::SprintStart);
	IC->BindAction(InputSprint, ETriggerEvent::Completed, this, &ASCharacter::SprintStop);
	
	IC->BindAction(InputAttack, ETriggerEvent::Started, this, &ASCharacter::PrimaryAttack);
	
	IC->BindAction(InputBlackHole, ETriggerEvent::Started, this, &ASCharacter::BlackHoleAttack);
	
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
	ActionComponent->StartActionByName(this, "PrimaryAttack");
}

void ASCharacter::BlackHoleAttack(const FInputActionValue& Value)
{
	ActionComponent->StartActionByName(this, "BlackHole");
}

void ASCharacter::SprintStart(const FInputActionValue& Value)
{
	ActionComponent->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop(const FInputActionValue& Value)
{
	ActionComponent->StopActionByName(this, "Sprint");
}


void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if(ensureAlways(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.Instigator = this;

		FCollisionShape Shape;
		Shape.SetSphere(20.f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = CameraComponent->GetComponentLocation();
		FVector TraceEnd = CameraComponent->GetComponentLocation() + (GetControlRotation().Vector() * 5000);

		FHitResult Hit;

		if(GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjRotator = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTM = FTransform(ProjRotator, HandLocation);
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParameters);
	}
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if(NewHealth <= 0 && Delta < 0.f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
		SetLifeSpan(5.0f);
	}
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
}

void ASCharacter::Interact(const FInputActionValue& Value)
{
	InteractionComp->PrimaryInteract();
}
