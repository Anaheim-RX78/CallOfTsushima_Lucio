// Copyright Epic Games, Inc. All Rights Reserved.

#include "CallOfTsushimaGameMode.h"
#include "CallOfTsushimaCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACallOfTsushimaGameMode::ACallOfTsushimaGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
