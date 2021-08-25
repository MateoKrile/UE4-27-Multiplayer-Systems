// Copyright Epic Games, Inc. All Rights Reserved.

#include "Multiplayer_CourseGameMode.h"
#include "Multiplayer_CourseCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultiplayer_CourseGameMode::AMultiplayer_CourseGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
