// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "EnumTeam.h"
#include "DB_TeamStart.generated.h"

/**
 * 
 */
UCLASS()
class DODGEBATTLE_API ADB_TeamStart : public APlayerStart
{
	GENERATED_BODY()

		// Team Spawn
		UPROPERTY(EditAnywhere, Category = team)
		TEnumAsByte<Team> currentTeam;
};
