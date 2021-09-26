// Fill out your copyright notice in the Description page of Project Settings.


#include "DB_CrosshairWidget.h"


void UDB_CrosshairWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDB_CrosshairWidget::UpdateOutLocation(FVector2D l, float scale)
{
	FWidgetTransform trans = RenderTransform;
	trans.Translation = l;
	SetRenderTransform(trans);
}

void UDB_CrosshairWidget::UpdateInLocation(FVector2D d, float scale)
{
}

