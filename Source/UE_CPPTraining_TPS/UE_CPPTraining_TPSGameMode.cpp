// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_CPPTraining_TPSGameMode.h"
#include "UE_CPPTraining_TPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE_CPPTraining_TPSGameMode::AUE_CPPTraining_TPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
