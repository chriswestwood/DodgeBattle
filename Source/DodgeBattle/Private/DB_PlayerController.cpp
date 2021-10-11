// Fill out your copyright notice in the Description page of Project Settings.

#include "DB_PlayerController.h"
#include "DB_Player.h"

void ADB_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ADB_Player* playerPawn = Cast<ADB_Player>(InPawn);
	if (playerPawn)
	{
		playerPawn->SetTeam(currentTeam);
	}
}

TEnumAsByte<Team> ADB_PlayerController::GetTeam()
{
	return currentTeam;
}

void ADB_PlayerController::SetTeam(TEnumAsByte<Team> newT)
{
	currentTeam = newT;
}
