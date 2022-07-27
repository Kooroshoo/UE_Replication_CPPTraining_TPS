// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NPC_Character.h"
#include "BehaviorTree/BehaviorTree.h"


ANPC_AIController::ANPC_AIController()
{
	BTComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComponent"));
	BBComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void ANPC_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ANPC_Character* NPC = Cast<ANPC_Character>(InPawn);

	if (NPC && NPC->NPCBT)
	{
		BBComp->InitializeBlackboard(*NPC->NPCBT->BlackboardAsset);

		NPCKeyID = BBComp->GetKeyID("PlayerPosition");

		BTComp->StartTree(*NPC->NPCBT);
	}
}
