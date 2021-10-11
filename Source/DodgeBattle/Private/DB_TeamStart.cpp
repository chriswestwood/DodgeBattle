// Fill out your copyright notice in the Description page of Project Settings.


#include "DB_TeamStart.h"

TEnumAsByte<Team> ADB_TeamStart::GetTeam()
{
	return currentTeam;
}

void ADB_TeamStart::SetTeam(TEnumAsByte<Team> newT)
{
	currentTeam = newT;
}

