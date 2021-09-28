// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DB_Ball.h"
#include "EnumTeam.h"
#include "EnumBall.h"
#include "DB_Player.generated.h"


UCLASS()
class DODGEBATTLE_API ADB_Player : public ACharacter
{
	GENERATED_BODY()

public:
	/* FUNCTIONS */
	// Sets default values for this character's properties
	ADB_Player();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Returns CameraBoom
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	// Returns FollowCamera
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class UDestructibleComponent* DestructMeshComp;

	TEnumAsByte<Team> GetTeam();

protected:
	/* FUNCTIONS */
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//
	void Dodge();
	//
	void ThrowCharge();
	//
	void Throw();
	//
	void Block();
	//
	void Recall();
	//
	void StopBlock();
	// Called for forwards/backward input 
	void MoveForward(float Value);
	/** Called for side to side input */
	void MoveRight(float Value);
	//
	UFUNCTION()
	void OnPlayerHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	//
	UFUNCTION()
	void PlayerDestruct(AActor* killActor, const FHitResult& Hit);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/* VARIABLES */

	/** Camera boom positioning the camera behind the character */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Throw Point */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* ThrowPoint;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

		float DodgeCooldown;
		float DodgeCooldownTimer;
		float moveSpeed;

		UPROPERTY(EditAnywhere, Category = Inventory)
		TSubclassOf<class ADB_Ball> ballBlueprint;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Team)
		TEnumAsByte<Team> team;

		TArray<TEnumAsByte<BallType>> ballInv;

		FVector_NetQuantize throwEndPoint;

		class ADB_PlayerHUD* HUD;

};
