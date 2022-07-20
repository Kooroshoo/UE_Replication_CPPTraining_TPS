// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicPill.h"

// Sets default values
AMagicPill::AMagicPill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PillEffectValue = 0.f;
	PillMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillMesh"));
	RootComponent = PillMesh;
	PillMesh->SetSimulatePhysics(true);

}

// Called when the game starts or when spawned
void AMagicPill::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMagicPill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
