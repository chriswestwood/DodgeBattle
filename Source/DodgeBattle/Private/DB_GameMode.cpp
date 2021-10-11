// Fill out your copyright notice in the Description page of Project Settings.


#include "DB_GameMode.h"
#include "DB_Player.h"
#include "UObject/ConstructorHelpers.h"
#include "DB_PlayerHUD.h"
#include "DB_GameState.h"
#include "DB_PlayerState.h"
#include "DB_SpectatorPawn.h"
#include "DB_PlayerController.h"
#include "DB_PlayerState.h"
#include "DB_TeamStart.h"

ADB_GameMode::ADB_GameMode()
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
	GameStateClass = ADB_GameState::StaticClass();
	PlayerStateClass = ADB_PlayerState::StaticClass();
	SpectatorClass = ADB_SpectatorPawn::StaticClass();
	PlayerControllerClass = ADB_PlayerController::StaticClass();
	PlayerStateClass = ADB_PlayerState::StaticClass();

	//bStartPlayersAsSpectators = true;
	bDelayedStart = true;
}

void ADB_GameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
	if (ADB_PlayerController* playerController = Cast<ADB_PlayerController>(NewPlayer))
	{
		if (ADB_TeamStart* teamStart = Cast<ADB_TeamStart>(NewPlayer->StartSpot))
		{
			playerController->SetTeam(teamStart->GetTeam());
		}
	}

}