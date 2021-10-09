// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DB_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DODGEBATTLE_API ADB_PlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void OnPossess(APawn* InPawn) override;
};
