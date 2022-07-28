// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTask.h"
#include "GameplayTask_SpawnParticles.generated.h"

/**
 * 
 */
UCLASS()
class UE_CPPTRAINING_TPS_API UGameplayTask_SpawnParticles : public UGameplayTask
{
	GENERATED_BODY()

public:
	static UGameplayTask_SpawnParticles* ConstructTask (TScriptInterface<IGameplayTaskOwnerInterface> TaskOwner, UParticleSystem* ParticleSystem, FVector Location);

	UPROPERTY()
	UParticleSystem* ParticleSystem;
	FVector Location;

	virtual void Activate() override;
	
};
