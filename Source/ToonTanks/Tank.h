// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

/**
 *
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent *SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent *Camera;

	bool IsOnGround = false;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector PushForce = FVector(0.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float PushForceThreshold = 0.1f;

public:
	ATank();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent *OverlappedComponent,
						AActor *OtherActor,
						UPrimitiveComponent *OtherComp,
						int32 OtherBodyIndex,
						bool bFromSweep,
						const FHitResult &SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent *OverlappedComponent,
					  AActor *OtherActor,
					  UPrimitiveComponent *OtherComp,
					  int32 OtherBodyIndex);

private:
	void Move(float Value);

	void Rotate(float Value);

	void RotateUpDown(float Value);
};
