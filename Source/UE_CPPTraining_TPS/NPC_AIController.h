// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPC_AIController.generated.h"

/**
 * 
 */
UCLASS()
class UE_CPPTRAINING_TPS_API ANPC_AIController : public AAIController
{
	GENERATED_BODY()
	
	UPROPERTY(Transient)
	class UBlackboardComponent* BBComp;

	UPROPERTY(Transient)
	class UBehaviorTreeComponent* BTComp;

public:
	ANPC_AIController();

	virtual void OnPossess(APawn* InPawn) override;

	uint8 NPCKeyID;
	
};
