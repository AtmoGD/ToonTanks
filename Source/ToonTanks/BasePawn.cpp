// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentHealth = MaxHealth;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
}

void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActiveCooldown > 0.f)
	{
		ActiveCooldown -= DeltaTime;
	}
}

void ABasePawn::RotateTurret(float Value)
{
	float DeltaRotation = Value * TurretRotateSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator Rotation(0.f, DeltaRotation, 0.f);
	TurretMesh->AddRelativeRotation(Rotation);
}

void ABasePawn::RotateTurretTo(FVector LookAtTarget)
{
	FVector LookAtTargetCleaned = FVector(LookAtTarget.X, LookAtTarget.Y, TurretMesh->GetComponentLocation().Z);
	FVector StartLocation = TurretMesh->GetComponentLocation();
	FRotator TurretRotation = FVector(LookAtTargetCleaned - StartLocation).Rotation();
	TurretMesh->SetWorldRotation(TurretRotation);
}

void ABasePawn::Fire()
{
	if (ActiveCooldown > 0.f)
		return;

	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	AActor *Projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, Rotation);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireParticle, SpawnLocation, FRotator::ZeroRotator);
	ActiveCooldown = FireCooldown;
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), FireShake, GetActorLocation(), 0.f, 2500.f, 1.f, false);
}

void ABasePawn::TakeDamageAmount(float Damage)
{
	CurrentHealth -= Damage;
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), HitShake, GetActorLocation(), 0.f, 2500.f, 1.f, false);
	OnTakeDamage();

	if (CurrentHealth <= 0)
	{
		IsAlive = false;
		Die();
	}
}