// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "ZombieAIController.h"
#include "FPSZombieCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "FPSZombieGameMode.h"
#include "Sound/SoundAttenuation.h"

// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing"));
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = GetMesh()->GetAnimInstance();

	
	SensingComp->OnSeePawn.AddDynamic(this, &AZombie::SeePawn);
	// Spawn zombie, zombie AI controller get erros so move code to here
    GetCharacterMovement()->MaxWalkSpeed = 60.f;
	AZombieAIController* ZombieController = Cast<AZombieAIController>(GetController());
	if (ZombieController)
	{
		ZombieController->Move();
	}
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AZombie::SeePawn(APawn* Pawn)
{
	if (Cast<AFPSZombieCharacter>(Pawn))
	{
		GetCharacterMovement()->MaxWalkSpeed = 360.f;
	}
}

void AZombie::Attack()
{
	if (M_ZombieAttack)
	{
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(M_ZombieAttack);
		}
	}
	if (AttackSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation());
	}
}

float AZombie::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) 
{
	if (bIsDeath)
	{
		return 0;
	}
	AnimInstance->Montage_Play(M_ZombieHitReaction);

	float DamageApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	DamageApply = FMath::Min(DamageApply, Health);
	Health -= DamageApply;
	
	if (Health <= 0)
	{
		bIsDeath = true;
	}
	
	if (bIsDeath)
	{
		// If AI death, do these 2 thing 
		// Detach AI from its controller
		DetachFromControllerPendingDestroy();
		// Disable capsule component for no collision
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		AFPSZombieGameMode* GameMode = Cast<AFPSZombieGameMode>(GetWorld()->GetAuthGameMode());
		GameMode->ZombieKilled();
	}

	return DamageApply;
}