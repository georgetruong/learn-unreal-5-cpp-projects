// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ATank::ATank() {
	PrimaryActorTick.bCanEverTick = true;

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArmComp->SetupAttachment(RootComponent);
    
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
}

void ATank::BeginPlay() {
    Super::BeginPlay();

    PlayerControllerRef = Cast<APlayerController>(GetController());
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if(PlayerControllerRef) {
        FHitResult HitResult;
        PlayerControllerRef->GetHitResultUnderCursor(
            ECollisionChannel::ECC_Visibility, 
            false,
            HitResult
        );
        DrawDebugSphere(
            GetWorld(), 
            HitResult.ImpactPoint,
            25,
            10,
            FColor::Red,
            false,
            -1.f
        );
    }

}

void ATank::Move(float Value) {
    FVector DeltaLocation = FVector::ZeroVector;
    double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    DeltaLocation.X = Value * Speed * DeltaTime;
    AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value) {
    FRotator DeltaRotation = FRotator::ZeroRotator;
    double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    DeltaRotation.Yaw = Value * TurnRate * DeltaTime;
    AddActorLocalRotation(DeltaRotation, true);
}