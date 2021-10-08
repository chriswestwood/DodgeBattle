// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "EnumTeam.h"
#include "EnumBall.h"
#include "DB_Ball.generated.h"

UCLASS()
class DODGEBATTLE_API ADB_Ball : public AActor
{
	GENERATED_BODY()

public:
	// INIT
	ADB_Ball();
	// Override Tick
	virtual void Tick(float DeltaTime) override;
	// Enable Replication
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	// Override EndPlay
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// Initialise the velocity
	void InitVelocity(const FVector velDirection);
	// Initialise the momentum
	void InitMomentum(const FVector momentumDirection);
	// sets the velocity
	UFUNCTION()
	void UpdateVelocity(float DeltaTime);

	// Returns the balls team
	TEnumAsByte<Team> GetTeam();
	// Sets the balls team
	UFUNCTION()
	void SetTeam(TEnumAsByte<Team> t);
	// replicate Team
	UFUNCTION()
	void OnRep_Team();
	// update the Material to show the correct colour
	void OnTeamUpdate();


	// Sets the ball to return to the throwing player
	UFUNCTION()
	void SetReturnToPlayer();



protected:

	// Called when Created
	virtual void BeginPlay() override;
	// On Hit Function
	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Throw Momentum
	UPROPERTY()
	FVector momentum;

	// Current Team
	UPROPERTY(ReplicatedUsing = OnRep_Team,EditAnywhere, BlueprintReadWrite, Category = Team)
	TEnumAsByte<Team> currentTeam;
	// Blue Material - for ally team
	UPROPERTY()
	UMaterialInterface* BlueMat;
	// Red Material - for enemy team
	UPROPERTY()
	UMaterialInterface* RedMat;

	// Ball Type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ball)
	TEnumAsByte<BallType> ballType;
	// Ball Mesh Component
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* ballMeshComp;
	// Ball Collision Component
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* ballCollisionComp;
	// Ball Movement Component.
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;
	// Homing Point
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* HomingPoint;
	// flag to state returning to player
	bool bReturnToPlayer;
	// Return Timer - if time passes will auto return to player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ball)
	float returnTimer;
	// amount of times the ball has bounced
	int hitCount;
	// number of times the ball bounces before returning to player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ball)
	int hitReturnCount;
};
