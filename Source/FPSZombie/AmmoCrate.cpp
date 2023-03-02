// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoCrate.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSZombieCharacter.h"

// Sets default values
AAmmoCrate::AAmmoCrate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->InitSphereRadius(100.f);
	RootComponent = SphereCollision;

	Crate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ammo Crate"));
	Crate->SetupAttachment(RootComponent);

	// If use root comp for overlap event but does not work, create another trigger volume to do that
	SphereCapsule = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Capsule"));
	SphereCapsule->InitSphereRadius(100.f);
	SphereCapsule->SetupAttachment(RootComponent);

	SphereCapsule->OnComponentBeginOverlap.AddDynamic(this, &AAmmoCrate::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AAmmoCrate::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAmmoCrate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAmmoCrate::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		AFPSZombieCharacter* Character = Cast<AFPSZombieCharacter>(OtherActor);
		if (Character)
		{
			Character->TotalAmmo += 10;
			Destroy();
		}
	}
}