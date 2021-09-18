// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EnumBall.generated.h"

UENUM()
enum BallType
{
	Normal    UMETA(DisplayName = "Normal"),
	Spike   UMETA(DisplayName = "Spike"),
	Cluster   UMETA(DisplayName = "Cluster")
};
/**
 * 
 */
UCLASS()
class DODGEBATTLE_API UEnumBall : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
