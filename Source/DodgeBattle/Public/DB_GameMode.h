// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DB_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class DODGEBATTLE_API ADB_GameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ADB_GameMode();

	// override player spawning
	virtual void RestartPlayer(AController* NewPlayer) override;

};
