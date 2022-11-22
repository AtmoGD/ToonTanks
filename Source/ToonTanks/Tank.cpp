// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Math/UnrealMathUtility.h"

ATank::ATank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(TurretMesh);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);

    GroundCheck = CreateDefaultSubobject<UBoxComponent>(TEXT("Ground Check"));
    GroundCheck->SetupAttachment(RootComponent);
}

void ATank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (PushForce.Length() > PushForceThreshold)
    {
        FVector PushForceToApply = PushForce * DeltaTime;
        FMath::Abs(PushForce.Length()) > 0.f ? PushForce -= PushForceToApply : PushForce = FVector(0.f, 0.f, 0.f);
        AddActorWorldOffset(PushForceToApply, true);
    }
}

void ATank::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ATank::Move);
    PlayerInputComponent->BindAxis("RotateTurret", this, &ATank::RotateTurret);
    PlayerInputComponent->BindAxis("RotateUpDown", this, &ATank::RotateUpDown);
    PlayerInputComponent->BindAxis("Turn", this, &ATank::Rotate);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATank::Fire);
}

void ATank::Move(float Value)
{
    if (PushForce.Length() > PushForceThreshold || !IsAlive)
        return;

    float Delta = GetWorld()->DeltaTimeSeconds;
    float CurrentGravity = IsGrounded() ? 0.f : Gravity * Delta;

    FVector DeltaLocation(Value * MoveSpeed * Delta, 0.f, CurrentGravity);
    AddActorLocalOffset(DeltaLocation);
    CurrentSpeed = Value;
}

void ATank::Rotate(float Value)
{
    FRotator DeltaRotation(0.f, Value * RotateSpeed * GetWorld()->DeltaTimeSeconds, 0.f);
    AddActorLocalRotation(DeltaRotation);
}

void ATank::RotateUpDown(float Value)
{
    float DeltaRotation = Value * RotateUpDownSpeed * GetWorld()->DeltaTimeSeconds;
    FRotator Rotation(DeltaRotation, 0.f, 0.f);
    FRotator SpringRotation = SpringArm->GetRelativeRotation();
    SpringRotation += Rotation;
    SpringRotation.Pitch = FMath::Clamp(SpringRotation.Pitch, RotateUpDownMin, RotateUpDownMax);
    SpringArm->SetRelativeRotation(SpringRotation);
}

bool ATank::IsGrounded()
{
    TArray<UPrimitiveComponent *> OverlappingActors;
    GroundCheck->GetOverlappingComponents(OverlappingActors);

    for (UPrimitiveComponent *OverlappingComponent : OverlappingActors)
    {
        AActor *OverlappingActor = OverlappingComponent->GetOwner();
        if (OverlappingActor->ActorHasTag("Ground"))
        {
            return true;
        }
    }

    return false;
}