// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChasePlayer.h"
#include "NPC_AIController.h"
#include "UE_CPPTraining_TPSCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANPC_AIController* ThisNPC = Cast<ANPC_AIController>(OwnerComp.GetAIOwner());

	/*
	// Chase the first player code
	AUE_CPPTraining_TPSCharacter* Player = Cast<AUE_CPPTraining_TPSCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Player)
	{
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(ThisNPC->NPCKeyID, Player->GetActorLocation());
	}
	*/
	
	
	if (ThisNPC)
	{
		FVector PlayerPos = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(ThisNPC->NPCKeyID);
		ThisNPC->MoveToLocation(PlayerPos, 5.f, true, true, false, true, 0, true);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
