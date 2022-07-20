// Fill out your copyright notice in the Description page of Project Settings.


#include "PillSpawner.h"
#include "MagicPill.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APillSpawner::APillSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawningVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawning Volume"));
	RootComponent = SpawningVolume;

	// this makes the item to spawn a magic pill
	//ItemToSpawn = AMagicPill::StaticClass();
}

// Called when the game starts or when spawned
void APillSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnPills();
	
}

// Called every frame
void APillSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector APillSpawner::GetRandomPointInVolume()
{
	FVector Origin = SpawningVolume->Bounds.Origin;
	FVector Extent = SpawningVolume->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
}

void APillSpawner::SpawnPills()
{
	if (ItemToSpawn != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FVector SpawnLocation = GetRandomPointInVolume();
			FRotator SpawnRotation;
			SpawnRotation.Pitch = FMath::FRand()*360.0f;
			SpawnRotation.Roll = FMath::FRand()*360.0f;
			SpawnRotation.Yaw = FMath::FRand()*360.0f;

			AMagicPill* SpawnedPill = World->SpawnActor<AMagicPill>(ItemToSpawn, SpawnLocation, SpawnRotation);
		}
	}
}

