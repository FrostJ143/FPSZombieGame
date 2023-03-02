// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSZombieGameMode.h"
#include "FPSZombieCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Zombie.h"
#include "ShooterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "AmmoCrate.h"

AFPSZombieGameMode::AFPSZombieGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void AFPSZombieGameMode::BeginPlay()
{
	StartGame();
}

void AFPSZombieGameMode::StartGame()
{
	SpawnZombie();
}

void AFPSZombieGameMode::SpawnZombie()
{
	SpawnAmmoCrate(AmmoLocationArray[0]);

	int TotalZombie = Wave * 4;
	ZombieLeft = TotalZombie;
	
	for (int i = 0; i < TotalZombie; i++)
	{
		if (ZombieToSpawn != nullptr)
		{ 
			UClass* const actorClassToSpawn = ZombieToSpawn.Get();
			if (actorClassToSpawn != NULL)
			{
				FActorSpawnParameters SpawnParams;
				AZombie* ZombieRef = nullptr;
				if (i < TotalZombie / 2)
				{
					ZombieRef = GetWorld()->SpawnActor<AZombie>(actorClassToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
				}
				else
				{
					ZombieRef = GetWorld()->SpawnActor<AZombie>(actorClassToSpawn, SpawnLocation2, SpawnRotation2, SpawnParams);
				}

				if (ZombieRef)
				{
					UE_LOG(LogTemp, Display, TEXT("Spawned"));
				}
			}
		}
	}
}

void AFPSZombieGameMode::ZombieKilled()
{
	ZombieLeft--;
	if (ZombieLeft == 0)
	{
		WaveIncrease();
	}
}

void AFPSZombieGameMode::WaveIncrease()
{
	if (Wave == MaxWave)
	{
		AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		if (PlayerController)
		{
			PlayerController->GameHasEnded(nullptr, true);
		}
	}
	else
	{
		Wave++;
		ShowWaveBox();
		SpawnZombie();
	}
}

void AFPSZombieGameMode::SpawnAmmoCrate(FVector Loc)
{
	// int RandomLocation = FMath::RandRange(0, 3);
	// FVector SpawnAmmoLocation = AmmoLocationArray[RandomLocation];
	FRotator SpawnAmmoRotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnParams;
	
	if (AmmoCrateToSpawn != NULL)
	{
		UE_LOG(LogTemp, Display, TEXT("%s"), *Loc.ToString());
		AAmmoCrate* AmmoCrateRef = nullptr;
		AmmoCrateRef = GetWorld()->SpawnActor<AAmmoCrate>(AmmoCrateToSpawn, Loc, SpawnAmmoRotation, SpawnParams);
		if (AmmoCrateRef)
		{
			UE_LOG(LogTemp, Display, TEXT("Spawn Ammo Crate"));
		}
	}

}