// Fill out your copyright notice in the Description page of Project Settings.


#include "DB_Platform.h"
#include "Net/UnrealNetwork.h"
#include "DestructibleComponent.h"
#include "DB_Ball.h"
#include "DB_Player.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

// Sets default values
ADB_Platform::ADB_Platform()
{
    bReplicates = true;
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
    
    // DEFAULTS
    currentTeam = Team::None;
    if (GetLocalRole() == ROLE_Authority)
    {
        DestructMeshComp->OnComponentHit.AddDynamic(this, &ADB_Platform::OnCompHit);
    }
}

// Called when the game starts or when spawned
void ADB_Platform::BeginPlay()
{
	Super::BeginPlay();
  
}

void ADB_Platform::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    ADB_Ball* hitBall = Cast<ADB_Ball>(OtherActor);
    if (hitBall)
    {
        if (hitBall->GetTeam() == currentTeam)
        {
            SetLifeSpan(5.0f);
            PlatformDestruct(OtherActor, Hit);            
        }
        else
        {                
            SetTeam(hitBall->GetTeam());
        }
    }
}

void ADB_Platform::PlatformDestruct_Implementation(AActor* killActor, const FHitResult& Hit)
{
    DestructMeshComp->SetSimulatePhysics(true);
    UGameplayStatics::ApplyPointDamage(this, 1.0f, Hit.ImpactNormal, Hit, GetInstigatorController(), killActor, UDamageType::StaticClass());  
}

// Called every frame
void ADB_Platform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADB_Platform::SetTeam(TEnumAsByte<Team> newT)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        currentTeam = newT;
        OnTeamUpdate(currentTeam);
    }
}

void ADB_Platform::OnRep_Team()
{
    OnTeamUpdate(currentTeam);
}

void ADB_Platform::OnTeamUpdate(TEnumAsByte<Team> compareTeam)
{
    if(currentTeam == Team::None) return;
    // get local player controller/character
    ADB_Player* localPlayer = Cast<ADB_Player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (localPlayer && currentTeam == localPlayer->GetTeam())
    {
        if (BlueMat) DestructMeshComp->SetMaterial(0, BlueMat);
    }
    else
    {
        if (RedMat) DestructMeshComp->SetMaterial(0, RedMat);
    }
}

void ADB_Platform::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ADB_Platform, currentTeam);
}
