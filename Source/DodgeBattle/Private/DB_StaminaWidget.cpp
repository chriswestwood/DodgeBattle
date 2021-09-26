// Fill out your copyright notice in the Description page of Project Settings.


#include "DB_StaminaWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UDB_StaminaWidget::NativeConstruct()
{
	Super::NativeConstruct();
	StaminaBar = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("StaminaImage"));
	if (StaminaBar)
	{
		StaminaBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void UDB_StaminaWidget::UpdateStaminaPercent(float percent)
{
	percent = FMath::Clamp(percent, 0.0f, 1.0f);
	if (StaminaBar)
	{
		SetRenderScale(FVector2D(percent,1.0f));
	}

}

