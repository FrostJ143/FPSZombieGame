// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSZombieCharacter.h"
#include "FPSZombieProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Chaos/ChaosEngineInterface.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterPlayerController.h"
#include "AmmoCrate.h"


//////////////////////////////////////////////////////////////////////////
// AFPSZombieCharacter

AFPSZombieCharacter::AFPSZombieCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	TurnRateGamepad = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->AttachToComponent(Mesh1P, FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("WeaponSocket")));
}

void AFPSZombieCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

}

//////////////////////////////////////////////////////////////////////////// Input

void AFPSZombieCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &AFPSZombieCharacter::OnPrimaryAction);
	
	// Bind reloading event
	PlayerInputComponent->BindAction("Reloading", IE_Pressed, this, &AFPSZombieCharacter::Reloading);

	// Bind ADS event
	PlayerInputComponent->BindAction("ADS", IE_Pressed, this, &AFPSZombieCharacter::ADS);
	PlayerInputComponent->BindAction("ADS", IE_Released, this, &AFPSZombieCharacter::StopADS);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AFPSZombieCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AFPSZombieCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AFPSZombieCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AFPSZombieCharacter::LookUpAtRate);
}

void AFPSZombieCharacter::OnPrimaryAction()
{
	if (CurrentAmmo == 0 || bIsReloading)
	{
		if (OutOfAmmoSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, OutOfAmmoSound, GetActorLocation());
		}
		return;
	}

	CurrentAmmo--;
	// Trigger the OnItemUsed Event
	// OnUseItem.Broadcast();
	if (GunFireAnim)
	{
		WeaponMesh->PlayAnimation(GunFireAnim, false);
	}

	if (IsAiming && M_AimFire)
	{
		Mesh1P->GetAnimInstance()->Montage_Play(M_AimFire);
	}
	else if (M_IdleFire)
	{
		Mesh1P->GetAnimInstance()->Montage_Play(M_IdleFire);
	}
	
	AController* PlayerController = GetController();
	FVector OutLocation;
	FRotator OutRotation;
	if (PlayerController)
	{
		PlayerController->GetPlayerViewPoint(OutLocation, OutRotation);
	}
	FVector EndLine = OutLocation + OutRotation.Vector() * 1000;
	
	FHitResult OutHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.bReturnPhysicalMaterial = true;
	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, OutLocation, EndLine, ECollisionChannel::ECC_GameTraceChannel2, Params);

	FVector ShotDirection = -OutRotation.Vector();
	AActor* HitActor = OutHit.GetActor();


	// Get hit surface type		
	const EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(OutHit.PhysMaterial.Get());
	const FName SurfaceName = *StaticEnum<EPhysicalSurface>()->GetAuthoredNameStringByValue(SurfaceType);

	if (HitActor != nullptr)
	{
		if (HitActor->ActorHasTag(FName(TEXT("Zombie"))))
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), P_Blood, OutHit.Location);
			if (SurfaceName.ToString() == "SurfaceType1")
			{
				Damage = 100.f;
			}
			else
			{
				Damage = 20.f;
			}
			UE_LOG(LogTemp, Display, TEXT("%f"), Damage);
		}
		else 
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, P_Default, OutHit.Location);
		}
		FPointDamageEvent DamageEvent(Damage, OutHit, ShotDirection, nullptr);
		HitActor->TakeDamage(Damage, DamageEvent, PlayerController, this);
	}
	
}

void AFPSZombieCharacter::ADS()
{
	IsAiming = true;
}

void AFPSZombieCharacter::StopADS()
{
	IsAiming = false;
}

void AFPSZombieCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnPrimaryAction();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AFPSZombieCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void AFPSZombieCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFPSZombieCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFPSZombieCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AFPSZombieCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

bool AFPSZombieCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AFPSZombieCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AFPSZombieCharacter::EndTouch);

		return true;
	}
	
	return false;
}

float AFPSZombieCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) 
{
	float DamageApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageApply = FMath::Min(DamageAmount, Health);

	Health -= DamageApply;
	
	if (Health <= 0)
	{
		AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		if (PlayerController)
		{
			PlayerController->GameHasEnded(nullptr, false);
		}
	}
	else if (Health < 100)
	{
		GetWorld()->GetTimerManager().SetTimer(RegenerateLifeTimerHandle, this, &AFPSZombieCharacter::RegenerateLife, 3, true);
		if (Health <= 50)
		{
			AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
			PlayerController->ShowBloodOverlay();
		}
	}
	
	
	return DamageApply;
}

void AFPSZombieCharacter::RegenerateLife()
{
	Health += 10.f;
	if (Health >= 100)
	{
		Health = 100.f;
		GetWorld()->GetTimerManager().ClearTimer(RegenerateLifeTimerHandle);
	}
	else if (Health > 50)
	{
		AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		PlayerController->HideBloodOverlay();
	}
	UE_LOG(LogTemp, Display, TEXT("%f"), Health);
}

void AFPSZombieCharacter::Reloading()
{
	if (CurrentAmmo == MaxClipSize || TotalAmmo == 0)
	{
		return;
	}
	
	bIsReloading = true;
	int NeedAmmo = MaxClipSize - CurrentAmmo;
	int AmmoReloading = FMath::Min(NeedAmmo, TotalAmmo);
	TotalAmmo -= AmmoReloading;
	CurrentAmmo += AmmoReloading;
	
	Mesh1P->GetAnimInstance()->Montage_Play(M_Reloading);
	WeaponMesh->PlayAnimation(GunReloading, false);
	GetWorldTimerManager().SetTimer(
	ReloadingTimerHandle, this, &AFPSZombieCharacter::AfterReloading, 2, false);
}

void AFPSZombieCharacter::AfterReloading()
{
	bIsReloading = false;
}
