// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APawnTank::APawnTank()
{
    //This creates the basic components of the tank blueprint, like a spring arm and camera 
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
	Super::BeginPlay();
    //This grabs a reference to the player controller to store within a variable
    PlayerControllerRef = Cast<APlayerController>(GetController());
}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    //Calling the rotate and move functions per frame
    Rotate();
    Move();

    //If there is a player controller in the scene, then the code executes. This is done to prevent a null result, which could lead to a crash
    if (PlayerControllerRef)
    {
        //This grabs the location the player clicked on screen so a missile can be fired towards that point by roating the turret on the tank
        FHitResult TraceHitResult;
        PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
        FVector HitLocation = TraceHitResult.ImpactPoint;

        RotateTurret(HitLocation);
    }
}

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    //This makes this script aware of the player inputs and assigns them to their functionality
    PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
    PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::Fire);
}

void APawnTank::CalculateMoveInput(float Value)
{   //This calculates how the player moves
    MoveDirection = FVector(Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);
}

void APawnTank::CalculateRotateInput(float Value)
{
    //This calculates how the player roates
    float RotationAmount = Value * RotateSpeed * GetWorld()->DeltaTimeSeconds;
    FRotator Rotation = FRotator(0, RotationAmount, 0);
    RotationDirection = FQuat(Rotation);
}

void APawnTank::Move()
{
    //This uses the calculation of movement to move
    AddActorLocalOffset(MoveDirection, true);
}

void APawnTank::Rotate()
{
    //This uses the calculation of rotation to rotate
    AddActorLocalRotation(RotationDirection, true);
}

void APawnTank::HandleDestruction()
{
    Super::HandleDestruction();

    //If the player's HP reaches zero, then this is called to destroy the tank, hide the obejct from view, and prevent it from moving
    bIsPlayerAlive = false;

    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
}

bool APawnTank::GetIsPlayerAlive()
{
    //A variable for checking if the player is alive
    return bIsPlayerAlive;
}