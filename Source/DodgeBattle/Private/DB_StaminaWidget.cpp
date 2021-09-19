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
	StaminaNum = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("StaminaNum"));
	if (StaminaNum)
	{
		StaminaNum->SetVisibility(ESlateVisibility::Visible);
		StaminaNum->SetText(FText::FromString(("Test")));
	}
}

void UDB_StaminaWidget::UpdateStaminaPercent(float percent)
{
	if (StaminaNum)
	{
		StaminaNum->SetText(FText::FromString((FString::SanitizeFloat(percent))));
	}

}

