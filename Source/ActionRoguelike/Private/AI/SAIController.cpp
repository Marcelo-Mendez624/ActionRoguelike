// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BT);

	APawn* MyPawn =  UGameplayStatics::GetPlayerPawn(this, 0);
	//
	// if(MyPawn)
	// {
	// 	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
	// 	GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	//
	// }
}
