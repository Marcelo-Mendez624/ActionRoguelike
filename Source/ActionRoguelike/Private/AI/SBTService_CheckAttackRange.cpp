// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	if(ensure(BB))
	{
		AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
		if(TargetActor)
		{
			AAIController* MyController =  OwnerComp.GetAIOwner();
			if(ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();
				if(ensure(AIPawn))
				{
					const float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());

					const bool bWithinRange = DistanceTo < 2000.f;
					
					bool bHasLOS= false;
					
					if(bWithinRange)
						bHasLOS = MyController->LineOfSightTo(TargetActor);

					BB->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLOS) );
				}
			}
		}
	}
}
