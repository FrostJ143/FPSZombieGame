// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSZombieCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

// Declaration of the delegate that will be called when the Primary Action is triggered
// It is declared as dynamic so it can be accessed also in Blueprints
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseItem);

UCLASS(config=Game)
class AFPSZombieCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* WeaponMesh;
	
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:
	AFPSZombieCharacter();

protected:
	virtual void BeginPlay();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float TurnRateGamepad;

	/** Delegate to whom anyone can subscribe to receive this event */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUseItem OnUseItem;

	UPROPERTY(BlueprintReadonly)
	bool IsAiming = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* M_AimFire;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* M_IdleFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* M_Reloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimationAsset* GunReloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimationAsset* GunFireAnim;
	
	UPROPERTY(EditAnywhere)
	class UParticleSystem* P_Blood;
	
	UPROPERTY(EditAnywhere)
	class UParticleSystem* P_Default;
	
	UPROPERTY(EditAnywhere)
	float Damage = 20.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentAmmo = 9;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalAmmo = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxClipSize = 9;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsReloading = false;
	
	UPROPERTY(EditAnywhere)
	class USoundBase* OutOfAmmoSound;
	
	FTimerHandle RegenerateLifeTimerHandle;
	FTimerHandle ReloadingTimerHandle;
	

protected:
	void AfterReloading();
	
	/** Fires a projectile. */
	void OnPrimaryAction();
	
	/** ADS*/
	void ADS();
	void StopADS();
	
	/** Regenerate life*/
	void RegenerateLife();
	
	// Reloading
	void Reloading();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);


public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }


};

