// Fill out your copyright notice in the Description page of Project Settings.


#include "DB_Ball.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "DB_Player.h"

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
		ProjectileMovementComponent->Bounciness = 1.0f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
	// SOUNDS
	static ConstructorHelpers::FObjectFinder<USoundCue> hitCue(TEXT("SoundCue'/Game/Assets/DB_BallHitSoundCue.DB_BallHitSoundCue'"));
	if (hitCue.Succeeded()) hitAudioCue = hitCue.Object;
	hitAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HitAudioComp"));
	hitAudioComponent->bAutoActivate = false;
	hitAudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ADB_Ball::BeginPlay()
{
	Super::BeginPlay();
	ballCollisionComp->OnComponentHit.AddDynamic(this, &ADB_Ball::OnCompHit);
	if (hitAudioCue->IsValidLowLevelFast()) {
		hitAudioComponent->SetSound(hitAudioCue);
	}
}

void ADB_Ball::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ADB_Player* hitPlayer = Cast<ADB_Player>(OtherActor);
	if (hitPlayer)
	{
		Destroy();
		return;
	}
	hitAudioComponent->Play();
}

// Called every frame
void ADB_Ball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

