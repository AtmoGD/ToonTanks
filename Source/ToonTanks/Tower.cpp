// Fill out your copyright notice in the Description page of Project Settings.

#include "Tower.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

void ATower::BeginPlay()
{
    Super::BeginPlay();

    CheckForPlayer();
}

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Tanks.Num() > 0)
    {
        ATank *ClosestTank = GetClosestTank();
        if (ClosestTank)
        {
            RotateTurretTo(ClosestTank->GetActorLocation());
            Fire();
            IsAiming = true;
        }
        else
        {
            IsAiming = false;
        }
    }
}

void ATower::CheckForPlayer()
{
    TArray<AActor *> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATank::StaticClass(), FoundActors);

    if (FoundActors.Num() > 0)
    {
        for (AActor *Actor : FoundActors)
        {
            Tanks.Add(Cast<ATank>(Actor));
        }
    }
}

ATank *ATower::GetClosestTank()
{
    ATank *ClosestTank = nullptr;
    float ClosestDistance = MaxDistance;

    for (ATank *Tank : Tanks)
    {
        float Distance = FVector::Dist(Tank->GetActorLocation(), GetActorLocation());
        if (Distance < ClosestDistance && Distance < MaxDistance)
        {
            FHitResult HitResult;
            FCollisionQueryParams CollisionQueryParams;
            CollisionQueryParams.AddIgnoredActor(this);
            CollisionQueryParams.AddIgnoredActor(Tank);
            bool bIsHit = GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), Tank->GetActorLocation(), ECC_Visibility, CollisionQueryParams);
            if (!bIsHit)
            {
                ClosestTank = Tank;
                ClosestDistance = Distance;
            }
        }
    }

    return ClosestTank;
}

// ATank *ATower::GetClosestTank()
// {
//     ATank *ClosestTank = nullptr;
//     float ClosestDistance = MaxDistance;

//     CheckForPlayerInRange();

//     for (ATank *Tank : TanksInRange)
//     {
//         float Distance = FVector::Dist(Tank->GetActorLocation(), GetActorLocation());

//         if (Distance < ClosestDistance)
//         {
//             FHitResult HitResult;

//             FCollisionQueryParams CollisionQueryParams;
//             CollisionQueryParams.AddIgnoredActor(this);
//             CollisionQueryParams.AddIgnoredActor(Tank);

//             FVector StartLocation = GetActorLocation();
//             FVector EndLocation = Tank->GetActorLocation();

//             bool bIsHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionQueryParams);

//             UE_LOG(LogTemp, Warning, TEXT("IsInSight: %s"), bIsHit ? TEXT("true") : TEXT("false"));
//             if (bIsHit)
//             {
//                 ClosestTank = Tank;
//                 ClosestDistance = Distance;
//             }
//         }
//     }

//     return ClosestTank;
// }

// void ATower::CheckForPlayerInRange()
// {
//     for (ATank *Tank : Tanks)
//     {
//         float Distance = FVector::Dist(Tank->GetActorLocation(), GetActorLocation());
//         if (Distance < MaxDistance)
//         {
//             if (!TanksInRange.Contains(Tank))
//             {
//                 TanksInRange.Add(Tank);
//             }
//         }
//         else
//         {
//             if (TanksInRange.Contains(Tank))
//             {
//                 TanksInRange.Remove(Tank);
//             }
//         }
//     }
// }

// bool ATower::TankIsInSight(ATank *Tank)
// {
//     FHitResult HitResult;

//     FCollisionQueryParams CollisionQueryParams;
//     CollisionQueryParams.AddIgnoredActor(this);
//     CollisionQueryParams.AddIgnoredActor(Tank);

//     FVector StartLocation = GetActorLocation();
//     FVector EndLocation = Tank->GetActorLocation();

//     bool bIsHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionQueryParams);

//     DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f);

//     return bIsHit;
// }