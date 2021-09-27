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
	// Sets default values for this actor's properties
	ADB_Ball();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitVelocity(const FVector velDirection);
	void InitMomentum(const FVector momentumDirection);
	void UpdateVelocity(float DeltaTime);
	TEnumAsByte<Team> GetTeam();

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UStaticMeshComponent* ballMeshComp;
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* ballCollisionComp;

	// Projectile movement component.
	UPROPERTY(VisibleAnywhere, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FVector momentum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Team)
	TEnumAsByte<Team> team;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ball)
	TEnumAsByte<BallType> ballType;

};
