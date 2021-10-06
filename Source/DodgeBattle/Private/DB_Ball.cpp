// Fill out your copyright notice in the Description page of Project Settings.


#include "DB_Ball.h"
#include "DB_Player.h"
#include "DB_Platform.h"

// Sets default values
ADB_Ball::ADB_Ball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	
	if (!ballCollisionComp)
	{
		// Use a sphere as a simple collision representation.
		ballCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Set the sphere's collision radius.
		ballCollisionComp->InitSphereRadius(15.0f);
		// Set the root component to be the collision component.
		RootComponent = ballCollisionComp;
	}
	if (!ballMeshComp)
	{
		ballMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
		ballMeshComp->AttachToComponent(ballCollisionComp, FAttachmentTransformRules::KeepRelativeTransform);
	}
	if (!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(ballCollisionComp);
		ProjectileMovementComponent->InitialSpeed = 3000.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.9f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}

	// Set Where the ball will "home" in on
	HomingPoint = CreateDefaultSubobject<USceneComponent>(TEXT("HomingPoint"));
	HomingPoint->SetupAttachment(RootComponent);

	hitReturnCount = 2;
	returnTimer = 2;
}

// Called when the game starts or when spawned
void ADB_Ball::BeginPlay()
{
	Super::BeginPlay();
	ballCollisionComp->OnComponentHit.AddDynamic(this, &ADB_Ball::OnCompHit);
	
}

void ADB_Ball::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	hitCount++;
	ADB_Platform* hitPlatform = Cast<ADB_Platform>(OtherActor);
	if (hitPlatform)
	{
		if (hitCount >= hitReturnCount && !bReturnToPlayer)SetReturnToPlayer();
		return;
	}
	// hit anything else destroy
	Destroy();
}

// Called every frame
void ADB_Ball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bReturnToPlayer)
	{
		returnTimer -= DeltaTime;
		if (returnTimer < 0)
		{
			SetReturnToPlayer();
		}
	}
}

void ADB_Ball::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ADB_Player* PlayerOwner = Cast<ADB_Player>(GetOwner());
	if (PlayerOwner)
	{
		PlayerOwner->RemoveCurrentBall();
	}
}

void ADB_Ball::InitVelocity(const FVector velDirection)
{
	ProjectileMovementComponent->Velocity = velDirection * ProjectileMovementComponent->InitialSpeed;
}

void ADB_Ball::InitMomentum(const FVector momentumDirection)
{
	momentum = momentumDirection;
}

void ADB_Ball::UpdateVelocity(float DeltaTime)
{
	//ProjectileMovementComponent->Velocity += momentum * DeltaTime;
}

TEnumAsByte<Team> ADB_Ball::GetTeam()
{
	return team;
}

void ADB_Ball::SetTeam(TEnumAsByte<Team> t)
{
	team = t;
}

void ADB_Ball::SetReturnToPlayer()
{
	bReturnToPlayer = true;
	ProjectileMovementComponent->bIsHomingProjectile = true;
	ProjectileMovementComponent->HomingTargetComponent = GetOwner()->GetRootComponent();
	ProjectileMovementComponent->HomingAccelerationMagnitude = 20000;

}

