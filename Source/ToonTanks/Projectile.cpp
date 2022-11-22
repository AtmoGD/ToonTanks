// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/BasePawn.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	MeshComp->SetupAttachment(CapsuleComp);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LifeTimeCounter >= LifeTime)
	{
		DestroyProjectile();
	}
	else
	{
		LifeTimeCounter += DeltaTime;
		Move();
	}
}

void AProjectile::Move()
{
	AddActorWorldOffset(GetActorForwardVector() * MoveSpeed * GetWorld()->DeltaTimeSeconds, true);
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent *OverlappedComponent,
								 AActor *OtherActor,
								 UPrimitiveComponent *OtherComp,
								 int32 OtherBodyIndex,
								 bool bFromSweep,
								 const FHitResult &SweepResult)
{

	if (OtherActor && OtherActor != this)
	{
		ABasePawn *Pawn = Cast<ABasePawn>(OtherActor);
		if (Pawn)
		{
			FVector HitNormal = SweepResult.ImpactNormal;
			FVector HitPoint = SweepResult.ImpactPoint;
			HitPlayer(Pawn, HitPoint, HitNormal);
			Pawn->TakeDamageAmount(Damage);
		}

		AProjectile *Projectile = Cast<AProjectile>(OtherActor);
		if (Projectile)
		{
			Projectile->DestroyProjectile();
		}

		DestroyProjectile();
	}
}

void AProjectile::DestroyProjectile()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitParticles, GetActorLocation());
	Destroy();
}
