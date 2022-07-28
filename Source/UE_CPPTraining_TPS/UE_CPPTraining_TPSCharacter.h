// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyInterface.h"
#include "GameplayTask.h"
#include "UE_CPPTraining_TPSCharacter.generated.h"

UCLASS(config=Game)
class AUE_CPPTraining_TPSCharacter : public ACharacter, public IMyInterface, public IGameplayTaskOwnerInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AUE_CPPTraining_TPSCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	//Override the interface
	FString GetTestName() override;

	// declare a function that must be implemented in c++
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interfaces")
	bool ReactToPlayerEntered();
	virtual bool ReactToPlayerEntered_Implementation() override;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	UFUNCTION(BlueprintNativeEvent, Category=Collision)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void PostInitializeComponents() override;

	virtual void Jump() override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void PickObjects();
	
	// value to determine if our character can pickup objects
	bool bPickupMode;

	//Health
	UPROPERTY(ReplicatedUsing=OnRep_HP)
	float HP;

	//UPROPERTY(Replicated)
	class UTextRenderComponent* HPText;

	UFUNCTION()
	void OnRep_HP();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Particle effect
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Character")
	UParticleSystemComponent* VisualEFX;

	// functions bound to delegate
	void MakeEFXVisible();
	void MakeEFXInvisibile();

	void UpdateHP(float val);

	virtual void Tick(float DeltaSeconds) override;
	
	
};

