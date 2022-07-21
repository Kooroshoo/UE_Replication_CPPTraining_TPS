// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UE_CPPTraining_TPSGameMode.generated.h"

// Declaring a delegate
DECLARE_DELEGATE(FStandardDelegateSigniture)

UCLASS(minimalapi)
class AUE_CPPTraining_TPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUE_CPPTraining_TPSGameMode();

	FStandardDelegateSigniture CharacterVisualEffectDelegateStart;
	FStandardDelegateSigniture CharacterVisualEffectDelegateStop;
};



