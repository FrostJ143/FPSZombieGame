// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAttackAnimNotify.h"
#include "Zombie.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FPSZombieCharacter.h"

void UZombieAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) 
{
    if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
    {
        AZombie* ZombieAI = Cast<AZombie>(MeshComp->GetOwner());
        FVector ZombieLocation = ZombieAI->GetActorLocation();
        FRotator ZombieRotation = ZombieAI->GetActorRotation();
        FVector LineEnd = ZombieLocation + ZombieRotation.Vector() * 30;

        TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes;
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
        TArray< AActor * > ActorsToIgnore;
        ActorsToIgnore.Add(MeshComp->GetOwner());

        TArray<AActor*> overlappedActors;
        UKismetSystemLibrary::SphereOverlapActors(GetWorld(),
        LineEnd,
        30,
        ObjectTypes,
        nullptr,
        ActorsToIgnore,
        overlappedActors);
        
        FHitResult OutHit;
        for (AActor* a : overlappedActors)
        {
            if (Cast<AFPSZombieCharacter>(a) != NULL)
            {
                FPointDamageEvent DamageEvent(20.f, OutHit, -ZombieRotation.Vector(), nullptr);
                a->TakeDamage(20.f, DamageEvent, ZombieAI->GetController(), ZombieAI);
            }
        }
    }
}
