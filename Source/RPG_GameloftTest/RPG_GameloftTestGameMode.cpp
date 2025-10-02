// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_GameloftTestGameMode.h"
#include "RPG_GameloftTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARPG_GameloftTestGameMode::ARPG_GameloftTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_Player"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
