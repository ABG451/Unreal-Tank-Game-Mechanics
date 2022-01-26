// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "PawnTank.h"

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
	Super::BeginPlay();
	//This sets the rate of fire of the turret based adjustable variables
    GetWorld()->GetTimerManager().SetTimer(FireRateTimer, this, &APawnTurret::CheckFireCondition, FireRate, true);
    //This sets the the player controlled object, the tank, as a known object to the turret, so it can perform its functions
    PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    //If the the tank is within the firing range of the turret, it will rotate to point at the tank
    if (!PlayerPawn || ReturnDistanceToPlayer() > TurretRange)
    {
        return;
    }

    RotateTurret(PlayerPawn->GetActorLocation());
}

void APawnTurret::CheckFireCondition()
{
    //If player = null || player is dead, so stop functionality
    if(!PlayerPawn || !PlayerPawn->GetIsPlayerAlive())
    {
        return;
    }

    //if player is in range || target aquired, so fire
    if (ReturnDistanceToPlayer() <= TurretRange)
    {
        Fire();
    }
}

float APawnTurret::ReturnDistanceToPlayer()
{
    //This grabs the distance from the turret to the player so it can be chacked later to see if it is in range
    if(!PlayerPawn)
    {
        return 0.0f;
    }

    return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
}

void APawnTurret::HandleDestruction()
{
    //This is called when the turret's HP is depleted to zero, at which point it is destroyed
    Super::HandleDestruction();
    Destroy();
}