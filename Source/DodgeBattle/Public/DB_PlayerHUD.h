// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DB_PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class DODGEBATTLE_API ADB_PlayerHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ADB_PlayerHUD();

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
		void UpdateCrosshair(FVector2D location, FVector2D direction, float distance);

	UFUNCTION()
		void UpdateStamina(float newStamina);

	UPROPERTY(EditDefaultsOnly, Category = widget)
		TSubclassOf<class UDB_CrosshairWidget> crosshairWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = widget)
		TSubclassOf<class UDB_StaminaWidget> staminaWidgetClass;
private:
	class UDB_CrosshairWidget* crosshairWidget;
	class UDB_StaminaWidget* staminaWidget;
	
};
