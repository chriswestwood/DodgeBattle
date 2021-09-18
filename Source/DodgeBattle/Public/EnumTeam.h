// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EnumTeam.generated.h"

UENUM()
enum Team
{
	None    UMETA(DisplayName = "NoTeam"),
	Team1   UMETA(DisplayName = "1"),
	Team2   UMETA(DisplayName = "2"),
	Team3   UMETA(DisplayName = "3"),
	Team4   UMETA(DisplayName = "4")
};
/**
 * 
 */
UCLASS()
class DODGEBATTLE_API UEnumTeam : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
