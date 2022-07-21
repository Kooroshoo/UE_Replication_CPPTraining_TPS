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

	// Declare custom event (allow to shield the broadcast execution of these events from other classes)
	DECLARE_EVENT(APillSpawner, FPlayerEntered)

	FPlayerEntered OnPlayerEntered;

private:
	//Box Component Spawning Volume
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawning", meta=(AllowPrivateAccess="true"))
	class UBoxComponent* SpawningVolume;

	// spawns magic pills
	void SpawnPills();

	// event when a actor enters the volume
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// event when a actor exits the volume
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

};
