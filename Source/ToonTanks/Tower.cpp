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

    CheckForPlayerInRange();

    for (ATank *Tank : TanksInRange)
    {
        float Distance = FVector::Dist(Tank->GetActorLocation(), GetActorLocation());

        if (Distance < ClosestDistance)
        {
            bool IsInSight = TankIsInSight(Tank);
            UE_LOG(LogTemp, Warning, TEXT("IsInSight: %s"), IsInSight ? TEXT("true") : TEXT("false"));
            if (TankIsInSight(Tank))
            {
                ClosestTank = Tank;
                ClosestDistance = Distance;
            }
        }
    }

    return ClosestTank;
}

void ATower::CheckForPlayerInRange()
{
    for (ATank *Tank : Tanks)
    {
        float Distance = FVector::Dist(Tank->GetActorLocation(), GetActorLocation());
        if (Distance < MaxDistance)
        {
            if (!TanksInRange.Contains(Tank))
            {
                TanksInRange.Add(Tank);
            }
        }
        else
        {
            if (TanksInRange.Contains(Tank))
            {
                TanksInRange.Remove(Tank);
            }
        }
    }
}

bool ATower::TankIsInSight(ATank *Tank)
{
    FHitResult HitResult;

    FCollisionQueryParams CollisionQueryParams;
    CollisionQueryParams.AddIgnoredActor(this);
    CollisionQueryParams.AddIgnoredActor(Tank);

    FVector StartLocation = GetActorLocation();
    FVector EndLocation = Tank->GetActorLocation();

    bool bIsHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionQueryParams);

    // Print the hit result
    if (bIsHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitResult.GetActor()->GetName());
    }

    DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f);

    return bIsHit;
}