// Fill out your copyright notice in the Description page of Project Settings.


#include "DB_PlayerHUD.h"
#include "DB_CrosshairWidget.h"
#include "DB_StaminaWidget.h"
#include "Blueprint/UserWidget.h"

ADB_PlayerHUD::ADB_PlayerHUD()
{

}
void ADB_PlayerHUD::DrawHUD()
{
	Super::DrawHUD();
}

void ADB_PlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	crosshairWidget = CreateWidget<UDB_CrosshairWidget>(GetWorld(),crosshairWidgetClass);
	/** Make sure widget was created */
	if (crosshairWidget)
	{
		/** Add it to the viewport */
		crosshairWidget->AddToViewport();
	}
	staminaWidget = CreateWidget<UDB_StaminaWidget>(GetWorld(),staminaWidgetClass);
	/** Make sure widget was created */
	if (staminaWidget)
	{
		/** Add it to the viewport */
		staminaWidget->AddToViewport();
		staminaWidget->SetRenderScale(FVector2D(0.0f, 1.0f));
	}
}

void ADB_PlayerHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ADB_PlayerHUD::UpdateCrosshair(FVector2D location, FVector2D direction, float distance)
{
	crosshairWidget->UpdateOutLocation(location,distance);
}

void ADB_PlayerHUD::UpdateStamina(float newStamina)
{
	staminaWidget->UpdateStaminaPercent(newStamina);
}
