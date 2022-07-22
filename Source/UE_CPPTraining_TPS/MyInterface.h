// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE_CPPTRAINING_TPS_API IMyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// interface c++ only functionality
	virtual FString GetTestName();

	// Interface that can be called from BP
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category=Interfaces)
	bool ReactToPlayerExited();

	// Interface that use Native Event (calling BP event from c++)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Interfaces)
	bool ReactToPlayerEntered();
};
