// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "FPSZombieCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();
    // Set the game mode in project setting and world setting
    AddMainWdiget();
    UUserWidget* CrosshairHUD = CreateWidget(this, CrosshairHUDClass);
    if (CrosshairHUD != nullptr)
    {
        CrosshairHUD->AddToViewport();
    }
}

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);
    if (bIsWinner)
    {
        UUserWidget* WinHUD = CreateWidget(this, WinHUDClass);
        if (WinHUD)
        {
            WinHUD->AddToViewport();
        }
    }
    else
    {
        UUserWidget* LoseHUD = CreateWidget(this, LoseHUDClass);
        if (LoseHUD)
        {
            LoseHUD->AddToViewport();
        }
    }
    // AFPSZombieCharacter* ShooterChar = Cast<AFPSZombieCharacter>(GetPawn());
    // ShooterChar->GetCharacterMovement()->DisableMovement();
    bShowMouseCursor = true; 
    bEnableClickEvents = true; 
    bEnableMouseOverEvents = true;           
    SetInputMode(FInputModeUIOnly());
}

