// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Components/Widget.h"
#include "EnumBall.h"
#include "DB_PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class DODGEBATTLE_API ADB_PlayerHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Widget)
	void UpdateCrosshair(FVector2D location, FVector2D centreLoc, float distance);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Widget)
	void UpdateStamina(float newStamina);


protected:


};
