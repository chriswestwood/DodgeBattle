// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumTeam.h"
#include "DB_Platform.generated.h"

UCLASS()
class DODGEBATTLE_API ADB_Platform : public AActor
{
	GENERATED_BODY()
	
	// Destruct comp
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class UDestructibleComponent* DestructMeshComp;

public:	

	// FUNCTIONS

	// Sets default values for this actor's properties
	ADB_Platform();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Enable Replication
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	// Change team 
	void SetTeam(TEnumAsByte<Team> newT = None);
	// Replicate LastHitTeam
	UFUNCTION()
	void OnRep_Team();
	// update the Material to show the correct colour
 	void OnTeamUpdate(TEnumAsByte<Team> compareTeam);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Destroy Platform destructable
	UFUNCTION(NetMulticast, Reliable)
	void PlatformDestruct(AActor* killActor, const FHitResult& Hit);

	// VARIABLES

	// Team the Ball belongs to
	UPROPERTY(ReplicatedUsing = OnRep_Team, EditAnywhere, BlueprintReadWrite, Category = Team)
	TEnumAsByte<Team> currentTeam;

	// Blue Material - for ally team
	UPROPERTY()
	UMaterialInterface* BlueMat;
	// Red Material - for enemy team
	UPROPERTY()
	UMaterialInterface* RedMat;

};
