// Fill out your copyright notice in the Description page of Project Settings.


#include "PillSpawner.h"
#include "MagicPill.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "UE_CPPTraining_TPSCharacter.h"
#include "UE_CPPTraining_TPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

// Sets default values
APillSpawner::APillSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawningVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawning Volume"));
	SpawningVolume->SetHiddenInGame(false);
	RootComponent = SpawningVolume;

	// this makes the item to spawn a magic pill
	//ItemToSpawn = AMagicPill::StaticClass();
}

// Called when the game starts or when spawned
void APillSpawner::BeginPlay()
{
	Super::BeginPlay();
	
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

void APillSpawner::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!HasAuthority())
	{
		return;
	}

	AUE_CPPTraining_TPSCharacter* IsOtherPlayer = Cast<AUE_CPPTraining_TPSCharacter>(OtherActor);
	if (IsOtherPlayer)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange, FString::Printf(TEXT("%s has entered the volume"), *OtherActor->GetName()));
		SpawnPills();

		UWorld* World = GetWorld();
		if (World)
		{
			AUE_CPPTraining_TPSGameMode* MyGameMode = Cast<AUE_CPPTraining_TPSGameMode>(UGameplayStatics::GetGameMode(World));

			if (MyGameMode)
			{
				MyGameMode->CharacterVisualEffectDelegateStart.Broadcast();
			}
		}

		OnPlayerEntered.Broadcast();

		for (TActorIterator<AActor> Itr (GetWorld(), AActor::StaticClass()); Itr; ++Itr)
		{
			AActor* Actor = *Itr;
			IMyInterface* MyInterfaceInstance = Cast<IMyInterface>(Actor);
			if (MyInterfaceInstance)
			{
				MyInterfaceInstance->Execute_ReactToPlayerEntered(Actor);
			}
		}
	}
}

void APillSpawner::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	AUE_CPPTraining_TPSCharacter* IsOtherPlayer = Cast<AUE_CPPTraining_TPSCharacter>(OtherActor);
	if (IsOtherPlayer)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange, FString::Printf(TEXT("%s has exited the volume"), *OtherActor->GetName()));

		UWorld* World = GetWorld();
		if (World)
		{
			AUE_CPPTraining_TPSGameMode* MyGameMode = Cast<AUE_CPPTraining_TPSGameMode>(UGameplayStatics::GetGameMode(World));

			if (MyGameMode)
			{
				MyGameMode->CharacterVisualEffectDelegateStop.Broadcast();
			}
		}
	}
}

