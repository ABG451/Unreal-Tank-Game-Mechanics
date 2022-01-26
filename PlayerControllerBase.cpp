// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"

void APlayerControllerBase::SetPlayerEnabledState(bool SetPlayerEnabled)
{
    //Enables or disables player control over the tank object
    if (SetPlayerEnabled)
    {
        GetPawn()->EnableInput(this);
    }
    else
    {
        GetPawn()->DisableInput(this);
    }

    bShowMouseCursor = SetPlayerEnabled;
}
