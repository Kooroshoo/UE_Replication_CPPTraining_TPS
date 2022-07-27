// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FindPlayer.h"

#include "NPC_AIController.h"
#include "UE_CPPTraining_TPSCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTService_FindPlayer::UBTService_FindPlayer()
{
	bCreateNodeInstance = true;
}

void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ANPC_AIController* NPCController = Cast<ANPC_AIController>(OwnerComp.GetAIOwner());
	if (NPCController)
	{
		AUE_CPPTraining_TPSCharacter* Player = Cast<AUE_CPPTraining_TPSCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (Player)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(NPCController->NPCKeyID, Player->GetActorLocation());
		}
	}
}
