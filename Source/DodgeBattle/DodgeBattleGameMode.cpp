// Copyright Epic Games, Inc. All Rights Reserved.

#include "DodgeBattleGameMode.h"
#include "DodgeBattleCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADodgeBattleGameMode::ADodgeBattleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
