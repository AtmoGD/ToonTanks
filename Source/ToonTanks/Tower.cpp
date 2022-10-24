// Fill out your copyright notice in the Description page of Project Settings.

#include "Tower.h"
// #include "Kismet/GameplayStatics.h"
// #include "ToonTanks/Tank.h"
ATower::ATower()
{
    PrimaryActorTick.bCanEverTick = true;
}
// ATower::ATower()
// {
//     // Super::ABasePawn();

//     // FindPlayerTank();
// }

// void ATower::Tick(float DeltaTime)
// {
//     Super::Tick(DeltaTime);

//     if (PlayerTank)
//     {
//         RotateTurretTo(PlayerTank->GetActorLocation());
//     }
// }

// void ATower::FindPlayerTank()
// {
//     PlayerTank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
// }