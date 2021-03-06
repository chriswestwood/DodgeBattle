// Fill out your copyright notice in the Description page of Project Settings.


#include "DB_Ball.h"
#include "DB_Player.h"
#include "DB_Platform.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADB_Ball::ADB_Ball()
{
	bReplicates = true;
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
	// MATERIALS
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> HexBlueMatInstance(TEXT("MaterialInstanceConstant'/Game/Assets/Materials/M_Tech_Hex_Tile_Blue.M_Tech_Hex_Tile_Blue'"));
	if (HexBlueMatInstance.Succeeded())
	{
		BlueMat = HexBlueMatInstance.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> HexRedMatInstance(TEXT("MaterialInstanceConstant'/Game/Assets/Materials/M_Tech_Hex_Tile_Red.M_Tech_Hex_Tile_Red'"));
	if (HexRedMatInstance.Succeeded())
	{
		RedMat = HexRedMatInstance.Object;
	}
	// SOUNDS
	static ConstructorHelpers::FObjectFinder<USoundCue> hitCue(TEXT("SoundCue'/Game/Assets/DB_BallHitSoundCue.DB_BallHitSoundCue'"));
	if (hitCue.Succeeded()) hitAudioCue = hitCue.Object;
	hitAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HitAudioComp"));
	hitAudioComponent->bAutoActivate = false;
	hitAudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Set Where the ball will "home" in on
	HomingPoint = CreateDefaultSubobject<USceneComponent>(TEXT("HomingPoint"));
	HomingPoint->SetupAttachment(RootComponent);
	hitReturnCount = 2;
	returnTimer = 2;

	if (GetLocalRole() == ROLE_Authority)
	{
		ballCollisionComp->OnComponentHit.AddDynamic(this, &ADB_Ball::OnCompHit);
	}
}

// Called when the game starts or when spawned
void ADB_Ball::BeginPlay()
{
	Super::BeginPlay();
	if (hitAudioCue->IsValidLowLevelFast()) {
		hitAudioComponent->SetSound(hitAudioCue);
	}
}

void ADB_Ball::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	hitCount++;
	ADB_Platform* hitPlatform = Cast<ADB_Platform>(OtherActor);
	if (hitPlatform)
	{
		hitAudioComponent->Play();
		if (HitEmitter)
		{
			FTransform transform = FTransform(UKismetMathLibrary::MakeRotationFromAxes(Hit.ImpactNormal, FVector(0, 0, 0), FVector(0, 0, 0)));
			transform.SetLocation(Hit.ImpactPoint);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEmitter, transform, true, EPSCPoolMethod::None, true);
		}
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
	return currentTeam;
}

void ADB_Ball::SetTeam(TEnumAsByte<Team> t)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		currentTeam = t;
		OnTeamUpdate();
	}
}

void ADB_Ball::OnRep_Team()
{
	OnTeamUpdate();
}

void ADB_Ball::OnTeamUpdate()
{
	if (currentTeam == Team::None) return;
	// get local player controller/character
	ADB_Player* localPlayer = Cast<ADB_Player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (localPlayer && currentTeam == localPlayer->GetTeam())
	{
		if (BlueMat) ballMeshComp->SetMaterial(0, BlueMat);
	}
	else
	{
		if (RedMat) ballMeshComp->SetMaterial(0, RedMat);
	}
}

void ADB_Ball::SetReturnToPlayer()
{
	bReturnToPlayer = true;
	ProjectileMovementComponent->bIsHomingProjectile = true;
	ProjectileMovementComponent->HomingTargetComponent = Cast<ADB_Player>(GetOwner())->GetReturnPoint();
	ProjectileMovementComponent->HomingAccelerationMagnitude = 20000;
}

void ADB_Ball::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADB_Ball, currentTeam);
}

