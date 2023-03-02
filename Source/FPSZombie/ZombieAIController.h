// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class FPSZOMBIE_API AZombieAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	void Move();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* M_ZombieDeath;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
		
	APawn* CharacterPawn;
	
};
