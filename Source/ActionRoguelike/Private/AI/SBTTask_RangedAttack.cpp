// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "SProjectileBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();

	if(MyController) 
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (MyPawn == nullptr) return EBTNodeResult::Failed;
		

		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));

		if(TargetActor == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Target actor"));
			return EBTNodeResult::Failed;
		}
		
		if(!USAttributeComponent::IsActorAlive(TargetActor)) return EBTNodeResult::Failed;
		
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		MuzzleRotation.Pitch += FMath::RandRange(0, 10);
		MuzzleRotation.Yaw += FMath::RandRange(-10, 10);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = MyPawn;

		AActor* NewProject = GetWorld()->SpawnActor<ASProjectileBase>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);

		return NewProject ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
		
	}

	
	return EBTNodeResult::Failed;
}
