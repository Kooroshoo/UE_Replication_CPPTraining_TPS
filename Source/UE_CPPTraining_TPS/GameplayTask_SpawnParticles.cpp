// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTask_SpawnParticles.h"
#include "Kismet/GameplayStatics.h"


UGameplayTask_SpawnParticles* UGameplayTask_SpawnParticles::ConstructTask(
	TScriptInterface<IGameplayTaskOwnerInterface> TaskOwner, UParticleSystem* ParticleSystem, FVector Location)
{
	UGameplayTask_SpawnParticles* Task = NewTask<UGameplayTask_SpawnParticles>(TaskOwner);
	if (Task)
	{
		Task->ParticleSystem = ParticleSystem;
		Task->Location = Location;
		
	}
	return Task;
}

void UGameplayTask_SpawnParticles::Activate()
{
	Super::Activate();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, Location);
}
