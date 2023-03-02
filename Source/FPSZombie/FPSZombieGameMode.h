// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSZombieGameMode.generated.h"

UCLASS(minimalapi)
class AFPSZombieGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay();
		
public:
	AFPSZombieGameMode();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int Wave = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	int ZombieLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int MaxWave = 3;
	
	void ZombieKilled();

	UFUNCTION(BlueprintCallable)
	void SpawnZombie();
	
	UFUNCTION(BlueprintImplementableEvent)
	void ShowWaveBox();

	UPROPERTY(EditAnywhere)
	TArray<FVector> AmmoLocationArray;

	UFUNCTION()
	void SpawnAmmoCrate(FVector Loc);

private:
	void StartGame();
	
	
	void WaveIncrease();
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AZombie> ZombieToSpawn;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AAmmoCrate> AmmoCrateToSpawn;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FVector SpawnLocation;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FRotator SpawnRotation;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FVector SpawnLocation2;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FRotator SpawnRotation2;
	
};



