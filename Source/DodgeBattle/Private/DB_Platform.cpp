// Fill out your copyright notice in the Description page of Project Settings.


#include "DB_Platform.h"
#include "Net/UnrealNetwork.h"
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

}

// Called when the game starts or when spawned
void ADB_Platform::BeginPlay()
{
	Super::BeginPlay();
	if (LastHitTeam != 0) hasBeenHit = true;
}

void ADB_Platform::OnRep_LastHitTeam()
{
    onUpdateLastHit();
}

void ADB_Platform::onUpdateLastHit()
{
    //Client-specific functionality
    //if (IsLocallyControlled())
    //{
    //    FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
    //    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

    //    if (CurrentHealth <= 0)
    //    {
    //        FString deathMessage = FString::Printf(TEXT("You have been killed."));
    //        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
    //    }
    //}

    ////Server-specific functionality
    //if (GetLocalRole() == ROLE_Authority)
    //{
    //    FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
    //    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
    //}

    //Functions that occur on all machines. 
    /*
        Any special functionality that should occur as a result of damage or death should be placed here.
    */
}

// Called every frame
void ADB_Platform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

