// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ToonTanks/Tank.h"
#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

/**
 *
 */
UCLASS()
class TOONTANKS_API ATower : public ABasePawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MaxDistance = 600.f;

private:
	TArray<class ATank *> Tanks;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	void CheckForPlayer();
	ATank *GetClosestTank();
};
