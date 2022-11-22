// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "Components/CapsuleComponent.h"
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
}

void ATank::BeginPlay()
{
    Super::BeginPlay();

    CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ATank::OnOverlapBegin);
    CapsuleComp->OnComponentEndOverlap.AddDynamic(this, &ATank::OnOverlapEnd);
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
    if (PushForce.Length() > PushForceThreshold)
        return;

    float Delta = GetWorld()->DeltaTimeSeconds;
    float CurrentGravity = IsOnGround ? 0.f : Gravity * Delta;

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

void ATank::OnOverlapBegin(UPrimitiveComponent *OverlappedComponent,
                           AActor *OtherActor,
                           UPrimitiveComponent *OtherComp,
                           int32 OtherBodyIndex,
                           bool bFromSweep,
                           const FHitResult &SweepResult)
{
    UE_LOG(LogTemp, Warning, TEXT("OnOverlapBegin"));
    UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s"), *OtherActor->GetName());
    // Log Tags of Overlapped Actor
    for (FName Tag : OtherActor->Tags)
    {
        UE_LOG(LogTemp, Warning, TEXT("Tag: %s"), *Tag.ToString());
    }
    if (OtherActor->ActorHasTag("Ground"))
    {
        IsOnGround = true;
    }
}

void ATank::OnOverlapEnd(UPrimitiveComponent *OverlappedComponent,
                         AActor *OtherActor,
                         UPrimitiveComponent *OtherComp,
                         int32 OtherBodyIndex)
{
    if (OtherActor->ActorHasTag("Ground"))
    {
        IsOnGround = false;
    }
}