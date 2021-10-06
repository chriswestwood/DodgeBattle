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
	
public:	
	// Sets default values for this actor's properties
	ADB_Platform();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Change team, and update the Material to show the correct colour
	void UpdateTeam(TEnumAsByte<Team> newT = None);
	//
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class UDestructibleComponent* DestructMeshComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	//
	UFUNCTION()
	void PlatformDestruct(AActor* killActor, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Team)
	TEnumAsByte<Team> LastHitTeam;

	UPROPERTY()
	UMaterialInterface* BlueMat;

	UPROPERTY()
	UMaterialInterface* RedMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	UParticleSystem* HitEmitter;

	UPROPERTY()
		class USoundCue* hitAudioCue;
	UPROPERTY()
		class UAudioComponent* hitAudioComponent;

};
