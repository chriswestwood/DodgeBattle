// Fill out your copyright notice in the Description page of Project Settings.


#include "DB_Player.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "DestructibleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DB_PlayerHUD.h"

// include draw debug helpers header file
#include "DrawDebugHelpers.h"


// Sets default values
ADB_Player::ADB_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->AddLocalOffset(FVector(0, 0, 75.0f), false);
	CameraBoom->SocketOffset = FVector(0,110,0);
	CameraBoom->TargetArmLength = 250.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Add the Throw Point - so we can defien where to spawn the ball from
	ThrowPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ThrowPoint"));
	ThrowPoint->SetupAttachment(RootComponent);
	ThrowPoint->SetRelativeLocation(FVector(10, 60, 60));
	
	if (!DestructMeshComp)
	{
		DestructMeshComp = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructMesh"));
		DestructMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	DodgeCooldown = 5;
	DodgeCooldownTimer = 0;
	moveSpeed = 1.0f;
}

void ADB_Player::BeginPlay()
{
	Super::BeginPlay();
	HUD = Cast<ADB_PlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ADB_Player::OnPlayerHit);
}

TEnumAsByte<Team> ADB_Player::GetTeam()
{
	return team;
}


void ADB_Player::Dodge()
{
	if (DodgeCooldownTimer <= 0)
	{
		DodgeCooldownTimer = DodgeCooldown;
	}
}

void ADB_Player::ThrowCharge()
{
	moveSpeed = 0.2f;
}

void ADB_Player::Throw()
{
	moveSpeed = 1.0f;
	FVector ballLocation = ThrowPoint->GetComponentLocation();
	FVector throwDirection = throwEndPoint - ballLocation;
	throwDirection.Normalize();
	FRotator throwRotation = throwDirection.Rotation();
	ADB_Ball* newBall = GetWorld()->SpawnActor<ADB_Ball>(ballBlueprint, ballLocation, throwRotation);
	newBall->SetOwner(this);
}

void ADB_Player::Block()
{
}

void ADB_Player::Recall()
{
}

void ADB_Player::StopBlock()
{
}

void ADB_Player::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value * moveSpeed);
	}
}

void ADB_Player::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value * moveSpeed);
	}
}

void ADB_Player::OnPlayerHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ADB_Ball* hitBall = Cast<ADB_Ball>(OtherActor);
	if (hitBall)
	{
		if (hitBall->GetTeam() == team)
		{			
			//Destroy();
		}
		else
		{
			PlayerDestruct(OtherActor,Hit);
		}
	}
}

void ADB_Player::PlayerDestruct(AActor* killActor, const FHitResult& Hit)
{
	DestructMeshComp->SetSimulatePhysics(true);
	DestructMeshComp->SetHiddenInGame(false);
	DestructMeshComp->ApplyRadiusDamage(1.0f, Hit.ImpactPoint,100.0f,500.0f,false);
	GetMesh()->SetHiddenInGame(true);
	DisableInput(Cast<APlayerController>(GetController()));
	SetLifeSpan(5.0f);
}

void ADB_Player::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADB_Player::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


// Called every frame
void ADB_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (DodgeCooldownTimer > 0)
	{
		DodgeCooldownTimer -= DeltaTime;
		if (DodgeCooldownTimer < 0) DodgeCooldownTimer = 0;
		if(HUD)HUD->UpdateStamina(DodgeCooldownTimer/DodgeCooldown);
	}

	// Calculate the vector the character is going to throw to
	FHitResult OutHit;
	FVector Start = FollowCamera->GetComponentLocation();
	FVector ForwardVector = FollowCamera->GetForwardVector();
	FVector End = ((ForwardVector * 5000.f) + Start);
	FCollisionQueryParams CollisionParams;	
	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
	{
		if (OutHit.bBlockingHit)
		{
			End = OutHit.ImpactPoint;
		}			
	}
	Start = ThrowPoint->GetComponentLocation();
	throwEndPoint = End;
	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
	{
		if (OutHit.bBlockingHit)
		{
			throwEndPoint = OutHit.ImpactPoint;
		}
	}
	
	FVector2D ScreenLocation;
	const APlayerController* const PlayerController = Cast<const APlayerController>(GetController());
	PlayerController->ProjectWorldLocationToScreen(throwEndPoint, ScreenLocation);
	if (HUD)HUD->UpdateCrosshair(ScreenLocation,ScreenLocation,1.0f);
}

// Called to bind functionality to input
void ADB_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ADB_Player::Jump);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ADB_Player::Dodge);
	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &ADB_Player::ThrowCharge);
	PlayerInputComponent->BindAction("Throw", IE_Released, this, &ADB_Player::Throw);
	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &ADB_Player::Block);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &ADB_Player::StopBlock);
	PlayerInputComponent->BindAxis("MoveForward", this, &ADB_Player::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADB_Player::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ADB_Player::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ADB_Player::LookUpAtRate);

}

