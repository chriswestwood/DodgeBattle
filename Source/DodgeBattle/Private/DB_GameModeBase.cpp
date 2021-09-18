// Fill out your copyright notice in the Description page of Project Settings.


#include "DB_GameModeBase.h"
#include "DB_Player.h"
#include "UObject/ConstructorHelpers.h"

ADB_GameModeBase::ADB_GameModeBase()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Actors/Character/DB_PlayerBP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	// use our custom HUD class
	static ConstructorHelpers::FClassFinder<ADB_PlayerHUD> PlayerHUDBPClass(TEXT("/Game/Actors/Widget/DB_PlayerHUDBP"));
	if (PlayerHUDBPClass.Class != NULL)
	{
		HUDClass = PlayerHUDBPClass.Class;
	}
}
