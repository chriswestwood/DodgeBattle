// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DB_CrosshairWidget.generated.h"

/**
 * 
 */
UCLASS()
class DODGEBATTLE_API UDB_CrosshairWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* ImageOut;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* ImageIn;
	void UpdateOutLocation(FVector2D l, float scale);
	void UpdateInLocation(FVector2D d, float scale);

};
