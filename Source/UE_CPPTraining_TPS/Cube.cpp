// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACube::ACube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh Creation
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	// Replication Settings
	bNetLoadOnClient = true;
	bReplicates = true;


}

void ACube::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ACube::OnOverlapBegin);
	
}



// Called when the game starts or when spawned
void ACube::BeginPlay()
{
	Super::BeginPlay();

	// Setup dynamic material
	Material = Mesh->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this, FName("MyDynMat"));
	Mesh->SetMaterial(0, DynamicMaterial);
	
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
	if (!HasAuthority())
	{
		return;
	}
	
	RandomColor = FLinearColor::MakeRandomColor();
	OnRep_RandomColor();

}


// Setting up property replication
void ACube::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACube, RandomColor);
}


void ACube::OnRep_RandomColor()
{
	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), RandomColor);
}


