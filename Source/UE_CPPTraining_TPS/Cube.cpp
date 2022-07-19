// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"

// Sets default values
ACube::ACube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

}

// Called when the game starts or when spawned
void ACube::BeginPlay()
{
	Super::BeginPlay();

	Material = Mesh->GetMaterial(0);
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
	Mesh->SetMaterial(0, DynamicMaterial);

	FLinearColor RandomColor = FLinearColor(255,0,0);
	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), RandomColor);
}

// Called every frame
void ACube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACube::OnOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Box OverlapBegin"));
}

void ACube::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ACube::OnOverlapBegin);
	
}

