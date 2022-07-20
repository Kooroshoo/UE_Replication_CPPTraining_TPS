// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PillSpawner.generated.h"

UCLASS()
class UE_CPPTRAINING_TPS_API APillSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APillSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Item to spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	TSubclassOf<class AMagicPill> ItemToSpawn;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category="Spawning")
	FVector GetRandomPointInVolume();

private:
	//Box Component Spawning Volume
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawning", meta=(AllowPrivateAccess="true"))
	class UBoxComponent* SpawningVolume;

	// spawns magic pills
	void SpawnPills();

};
