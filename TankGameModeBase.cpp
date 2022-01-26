// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"

void ATankGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    HandleGameStart();
}

void ATankGameModeBase::ActorDied(AActor* DeadActor)
{

    if (DeadActor == PlayerTank)
    {
        //If the tank's HP reaches zero, it is destroyed, then the player's controll is disabled
        PlayerTank->HandleDestruction();
        HandleGameOver(false);

        if (PlayerControllerRef)
        {
            PlayerControllerRef->SetPlayerEnabledState(false);
        }
    }
    //If a turret's HP reaches zero, the it is destroyed, and the total number of remaining turrets is checked. If the ammount is zero, the game ends
    else if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor))
    {
        DestroyedTurret->HandleDestruction();

        if (--TargetTurrets == 0)
        {
            HandleGameOver(true);
        }
    }
}

void ATankGameModeBase::HandleGameStart()
{
    //This stores the ammount of turrets in the scene
    TargetTurrets = GetTargetTurretCount();
    //This stores the tank object as a variable
    PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
    //This grabs a refernece to the player controller and stores it as a variable
    PlayerControllerRef = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));

    GameStart();

    //Null check, the rest of code will not execute there is no player controller
    if (PlayerControllerRef)
    {
        PlayerControllerRef->SetPlayerEnabledState(false);
        //This calculates the countdown at the start of the game before the player can start controlling the tank
        FTimerHandle PlayerEnableHandle;
        FTimerDelegate  PlayerEnableDelegate = FTimerDelegate::CreateUObject(PlayerControllerRef, 
            &APlayerControllerBase::SetPlayerEnabledState, true);
        GetWorld()->GetTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDelay, false);
    }
}

void ATankGameModeBase::HandleGameOver(bool PlayerWon)
{
    GameOver(PlayerWon);
}

int32 ATankGameModeBase::GetTargetTurretCount()
{
    //This grabs all the turret objects in the scene and returns the ammount as a variable
    TArray<AActor*> TurretActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnTurret::StaticClass(), TurretActors);

    return TurretActors.Num();
}
