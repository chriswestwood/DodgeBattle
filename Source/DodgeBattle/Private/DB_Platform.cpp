// Fill out your copyright notice in the Description page of Project Settings.


#include "DB_Platform.h"
#include "Net/UnrealNetwork.h"
#include "DestructibleComponent.h"
#include "DB_Ball.h"
#include "DB_Player.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Engine/Engine.h"

// Sets default values
ADB_Platform::ADB_Platform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    if (!RootComponent)
    {
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    }
    if (!DestructMeshComp)
    {
        DestructMeshComp = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructMesh"));
        DestructMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    }
    // MATERIALS
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> HexBlueMatInstance(TEXT("MaterialInstanceConstant'/Game/Assets/M_Tech_Hex_Tile_Pulse_Blue.M_Tech_Hex_Tile_Pulse_Blue'"));
    if (HexBlueMatInstance.Succeeded())
    {
        BlueMat = HexBlueMatInstance.Object;
    }
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> HexRedMatInstance(TEXT("MaterialInstanceConstant'/Game/Assets/M_Tech_Hex_Tile_Pulse_Red.M_Tech_Hex_Tile_Pulse_Red'"));
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
}

// Called when the game starts or when spawned
void ADB_Platform::BeginPlay()
{
	Super::BeginPlay();
    DestructMeshComp->OnComponentHit.AddDynamic(this, &ADB_Platform::OnCompHit);
    if (hitAudioCue->IsValidLowLevelFast()) {
        hitAudioComponent->SetSound(hitAudioCue);
    }
}

void ADB_Platform::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    ADB_Ball* hitBall = Cast<ADB_Ball>(OtherActor);
    if (hitBall)
    {
        hitAudioComponent->Play();
        if (HitEmitter)
        {
            FTransform transform = FTransform(UKismetMathLibrary::MakeRotationFromAxes(Hit.ImpactNormal, FVector(0, 0, 0), FVector(0, 0, 0)));
            transform.SetLocation(Hit.ImpactPoint);
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEmitter, transform, true, EPSCPoolMethod::None, true);
        }
        if (hitBall->GetTeam() == LastHitTeam)
        {
            PlatformDestruct(OtherActor, Hit);            
        }
        else
        {                
            UpdateTeam(hitBall->GetTeam());
        }
    }
}

void ADB_Platform::PlatformDestruct(AActor* killActor, const FHitResult& Hit)
{
    DestructMeshComp->SetSimulatePhysics(true);
    UGameplayStatics::ApplyPointDamage(this, 1.0f, Hit.ImpactNormal, Hit, GetInstigatorController(), killActor, UDamageType::StaticClass());
    SetLifeSpan(5.0f);
}
// Called every frame
void ADB_Platform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADB_Platform::UpdateTeam(TEnumAsByte<Team> newT)
{
    if(newT != None) LastHitTeam = newT;    
    if (LastHitTeam == Team::None)
    {
        return;
    }
    // get local player controller/character
    ADB_Player* localPlayer = Cast<ADB_Player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (localPlayer && LastHitTeam == localPlayer->GetTeam())
    {
        if (BlueMat) DestructMeshComp->SetMaterial(0, BlueMat);
    }
    else
    {
        if (RedMat) DestructMeshComp->SetMaterial(0, RedMat);
    }
}

