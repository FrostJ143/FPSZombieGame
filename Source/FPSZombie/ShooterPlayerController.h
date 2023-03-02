// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FPSZOMBIE_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay();

public:
	UFUNCTION(BlueprintImplementableEvent)
	void AddMainWdiget();
	
	virtual void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowBloodOverlay();
	
	UFUNCTION(BlueprintImplementableEvent)
	void HideBloodOverlay();

		
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> CrosshairHUDClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseHUDClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinHUDClass;
};
