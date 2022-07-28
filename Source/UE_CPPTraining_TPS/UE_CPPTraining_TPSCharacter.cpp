// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_CPPTraining_TPSCharacter.h"

#include <string>

#include "GameplayTask_SpawnParticles.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "UE_CPPTraining_TPSGameMode.h"
#include "MagicPill.h"
#include "Components/TextRenderComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Net/UnrealNetwork.h"

//////////////////////////////////////////////////////////////////////////
// AUE_CPPTraining_TPSCharacter

AUE_CPPTraining_TPSCharacter::AUE_CPPTraining_TPSCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Setting up the Particle system
	VisualEFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("VisEFX"));
	VisualEFX->AttachTo(RootComponent);
	auto ParticleSystem = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Sparks.P_Sparks'"));
	if (ParticleSystem.Object != nullptr)
	{
		VisualEFX->SetTemplate(ParticleSystem.Object);
	}
	VisualEFX->SetVisibility(false);
	VisualEFX->SetIsReplicated(true);

	bPickupMode = true;

	// Max Health
	HP = 750.0f;

	HPText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HP Text"));
	HPText->AttachTo(RootComponent);
	HPText->SetText(FText::FromString(FString::Printf(TEXT("HP: %.1f"), HP)));
	HPText->SetRelativeLocation(FVector(0, -30, 75));
	HPText->SetRelativeRotation(FRotator(0, 180.f, 0));
	HPText->SetXScale(1.f);
	HPText->SetYScale(1.f);
	HPText->SetWorldSize(15);

	// Dynamic Material
	UMaterialInstanceConstant* TheConstMatInst = ConstructorHelpers::FObjectFinderOptional<UMaterialInstanceConstant>(TEXT("MaterialInstanceConstant'/Game/Mannequin/Character/Materials/MI_Male_Body_Dynamic_Inst.MI_Male_Body_Dynamic_Inst'")).Get();
	UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(TheConstMatInst, this->GetMesh());
	this->GetMesh()->SetMaterial(0, DynamicMaterialInstance);
	


}

//////////////////////////////////////////////////////////////////////////
// Input

void AUE_CPPTraining_TPSCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &AUE_CPPTraining_TPSCharacter::PickObjects);

	PlayerInputComponent->BindAxis("MoveForward", this, &AUE_CPPTraining_TPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUE_CPPTraining_TPSCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AUE_CPPTraining_TPSCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AUE_CPPTraining_TPSCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AUE_CPPTraining_TPSCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AUE_CPPTraining_TPSCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AUE_CPPTraining_TPSCharacter::OnResetVR);
}


FString AUE_CPPTraining_TPSCharacter::GetTestName()
{
	FString MyName = this->GetName();
	return MyName;
}

// Implementation of the react to player entered event
bool AUE_CPPTraining_TPSCharacter::ReactToPlayerEntered_Implementation()
{
	//HP -= 100;
	UpdateHP(-100);
	HPText->SetText(FText::FromString(FString::Printf(TEXT("HP: %.1f"), HP)));
	return true;
}


void AUE_CPPTraining_TPSCharacter::OnResetVR()
{
	// If UE_CPPTraining_TPS is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in UE_CPPTraining_TPS.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AUE_CPPTraining_TPSCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AUE_CPPTraining_TPSCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AUE_CPPTraining_TPSCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUE_CPPTraining_TPSCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AUE_CPPTraining_TPSCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AUE_CPPTraining_TPSCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AUE_CPPTraining_TPSCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (RootComponent)
	{
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AUE_CPPTraining_TPSCharacter::OnOverlapBegin);
	}
	
	
}

void AUE_CPPTraining_TPSCharacter::Jump()
{
	Super::Jump();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Player Jumped"));
}

void AUE_CPPTraining_TPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		AUE_CPPTraining_TPSGameMode* MyGameMode = Cast<AUE_CPPTraining_TPSGameMode>(UGameplayStatics::GetGameMode(World));

		if (MyGameMode)
		{
			MyGameMode->CharacterVisualEffectDelegateStart.AddUObject(this, &AUE_CPPTraining_TPSCharacter::MakeEFXVisible);
			MyGameMode->CharacterVisualEffectDelegateStop.AddUObject(this, &AUE_CPPTraining_TPSCharacter::MakeEFXInvisibile);
		}
	}


}

void AUE_CPPTraining_TPSCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UWorld* World = GetWorld();
	if (World)
	{
		AUE_CPPTraining_TPSGameMode* MyGameMode = Cast<AUE_CPPTraining_TPSGameMode>(UGameplayStatics::GetGameMode(World));

		if (MyGameMode)
		{
			MyGameMode->CharacterVisualEffectDelegateStart.RemoveAll(this);
			MyGameMode->CharacterVisualEffectDelegateStop.RemoveAll(this);
		}
	}

	
}

void AUE_CPPTraining_TPSCharacter::PickObjects()
{
	bPickupMode = !bPickupMode;
	if (bPickupMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("Pickup mode ON")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("Pickup mode OFF")));
	}
	
}

void AUE_CPPTraining_TPSCharacter::OnRep_HP()
{
	HPText->SetText(FText::FromString(FString::Printf(TEXT("HP: %.1f"), HP)));

	UMaterialInstanceDynamic* DynamicMaterialInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	float UpdateMatInstAmount = 1 - HP/750;
	DynamicMaterialInstance->SetScalarParameterValue("Amount", UpdateMatInstAmount);
}

void AUE_CPPTraining_TPSCharacter::MakeEFXVisible()
{
	VisualEFX->SetVisibility(true);
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("name: %s"), *this->GetName()));
}

void AUE_CPPTraining_TPSCharacter::MakeEFXInvisibile()
{
	VisualEFX->SetVisibility(false);
}

void AUE_CPPTraining_TPSCharacter::UpdateHP(float val)
{
	HP += val;

	if (HP < 0)
	{
		HP = 0;
	}
	if (HP > 750)
	{
		HP = 750;
	}

	float UpdateMatInstAmount = 1 - HP/750;
	UMaterialInstanceDynamic* DynamicMaterialInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	DynamicMaterialInstance->SetScalarParameterValue("Amount", UpdateMatInstAmount);
}

void AUE_CPPTraining_TPSCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Rotate the HPText
	/*
	FRotator HPRotator(0, 1.5f, 0);
	HPText->AddLocalRotation(HPRotator);
	*/
}


void AUE_CPPTraining_TPSCharacter::OnOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComponent,
                                                                 AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                                 const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		return;
	}
	
	AMagicPill* IsPill = Cast<AMagicPill>(OtherActor);
	if (IsPill != nullptr)
	{
		if (bPickupMode)
		{
			//HP += IsPill->PillEffectValue;
			UpdateHP(IsPill->PillEffectValue);
			IsPill->Destroy();
			OnRep_HP();
		}

	}
}

// Setting up property replication
void AUE_CPPTraining_TPSCharacter::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUE_CPPTraining_TPSCharacter, HP);
	
}
