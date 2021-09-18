// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleComponent.h"
#include "EnumTeam.h"
#include "DB_Platform.generated.h"

UCLASS()
class DODGEBATTLE_API ADB_Platform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADB_Platform();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	UDestructibleComponent* DestructMeshComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_LastHitTeam();

	void onUpdateLastHit();


	bool hasBeenHit;

	int LastHitTeam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Team)
	TEnumAsByte<Team> team;
};
