// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h"
#include "TankAimingComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed, bool DebugLine) {

	if (!Barrel) { return; }

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Fire"));

	// Calculate the OutLaunchVelocity
	bool bHaveAimSolution;
	if (bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace,
		FCollisionResponseParams::DefaultResponseParam,
		TArray<AActor*>(),
		DebugLine)) {

		FVector AimDirection = OutLaunchVelocity.GetSafeNormal();

		MoveBarrelTowards(AimDirection);

		auto time = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("%f Aim Solution found"), time);
		return;
	}

		auto time = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("%f No aim Solution found"), time);
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection) {

	// Work out difference between current barrel rotation and aim direction
	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotator = AimDirection.Rotation();
	FRotator DeltaRotator = AimAsRotator - BarrelRotator;
	auto time = GetWorld()->GetTimeSeconds();

	Barrel->Elevate(DeltaRotator.Pitch);
	// move the barrel the right amount 
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel * BarrelToSet) {

	Barrel = BarrelToSet;
}
