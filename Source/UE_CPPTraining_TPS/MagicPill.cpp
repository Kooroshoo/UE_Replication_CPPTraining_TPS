// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicPill.h"
#include "PillSpawner.h"
#include "EngineUtils.h"


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

	// get the PillSpawner Actor
	auto Itr = TActorIterator<APillSpawner>(GetWorld());
	TriggerEventSource = *Itr;
	if (TriggerEventSource)
	{
		TriggerEventSource->OnPlayerEntered.AddUObject(this, &AMagicPill::OnTriggerEvent);
	}
}

// Called every frame
void AMagicPill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMagicPill::OnTriggerEvent()
{
	PillEffectValue = FMath::RandRange(-150.f, 150.f);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("%f is my new value"), PillEffectValue));
}

