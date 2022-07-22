// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicPill.generated.h"

UCLASS()
class UE_CPPTRAINING_TPS_API AMagicPill : public AActor
{
	GENERATED_BODY()



	
public:	
	// Sets default values for this actor's properties
	AMagicPill();

	// trigger source (Pill spawner) for our custom event 
	UPROPERTY()
	class APillSpawner* TriggerEventSource;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// The amount of health/damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Values")
	float PillEffectValue;

protected:



public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* PillMesh;

	// this function is randomizing the pill effect
	UFUNCTION()
	void OnTriggerEvent();

};
