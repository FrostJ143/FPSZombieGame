// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Zombie.h"

void AZombieAIController::BeginPlay()
{
    Super::BeginPlay();

    CharacterPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    SetFocus(CharacterPawn, EAIFocusPriority::Gameplay);
    
    Move();
}

void AZombieAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AZombieAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);
    
    if (Result.Code == EPathFollowingResult::Success) 
    {
        AZombie* ZombiePawn = Cast<AZombie>(GetPawn());
        ZombiePawn->Attack();
        FTimerHandle UnusedHandle;
        GetWorldTimerManager().SetTimer(
        UnusedHandle, this, &AZombieAIController::Move, 2, false);
    }
}

void AZombieAIController::Move()
{
    MoveToActor(CharacterPawn);
}

