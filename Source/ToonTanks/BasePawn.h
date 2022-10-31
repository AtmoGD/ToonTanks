// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraShakeBase.h"
#include "ToonTanks/Projectile.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class TOONTANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent *CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent *BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent *TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent *ProjectileSpawnPoint;

	float ActiveCooldown = 0.f;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TurretRotateSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotateSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotateUpDownMin = -80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotateUpDownMax = -5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotateUpDownSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float FireCooldown = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	float CurrentHealth = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem *FireParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TSubclassOf<class UCameraShakeBase> FireShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TSubclassOf<class UCameraShakeBase> HitShake;

public:
	ABasePawn();

	virtual void Tick(float DeltaTime) override;

	void RotateTurret(float Value);

	void RotateTurretTo(FVector LookAtTarget);

	void TakeDamageAmount(float DamageAmount);

	UFUNCTION(BlueprintImplementableEvent)
	void OnTakeDamage();

	void Die();

	void Fire();

protected:
	virtual void BeginPlay() override;
};