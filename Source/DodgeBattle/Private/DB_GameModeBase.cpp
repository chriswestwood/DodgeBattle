// Fill out your copyright notice in the Description page of Project Settings.


#include "DB_GameModeBase.h"
#include "DB_Player.h"
#include "UObject/ConstructorHelpers.h"
#include "DB_PlayerHUD.h"

ADB_GameModeBase::ADB_GameModeBase()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Actors/Character/DB_PlayerBP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	// use the custom HUD class
	static ConstructorHelpers::FClassFinder<AHUD> PlayerHUDClass(TEXT("/Game/Actors/DB_PlayerHUDBP"));
	if (PlayerHUDClass.Class != NULL)
	{
		HUDClass = PlayerHUDClass.Class;
	}
}
