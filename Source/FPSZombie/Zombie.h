// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Zombie.generated.h"

UCLASS()
class FPSZOMBIE_API AZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void Attack();

	UPROPERTY(BlueprintReadOnly)
	bool bIsDeath = false;

private:
	UPROPERTY(EditAnywhere)
	class UPawnSensingComponent* SensingComp;
	
	// If want to use delegate function need to add UFUNCTION()
	UFUNCTION()
	void SeePawn(APawn* Pawn);
	
	UPROPERTY(EditAnywhere)
	class UAnimMontage* M_ZombieAttack;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* M_ZombieHitReaction;
	
	UPROPERTY()
	class UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere)
	class USoundBase* AttackSound;

		
	UPROPERTY()
	float Health = 100.f;
	
};
