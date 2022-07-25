// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cube.generated.h"

UCLASS()
class UE_CPPTRAINING_TPS_API ACube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY()
	UMaterialInterface* Material;

	UPROPERTY(ReplicatedUsing=OnRep_RandomColor)
	FLinearColor RandomColor;
	
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

	UFUNCTION(BlueprintNativeEvent, Category=Collision)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void PostInitializeComponents() override;

private:
	UFUNCTION()
	void OnRep_RandomColor();
};
