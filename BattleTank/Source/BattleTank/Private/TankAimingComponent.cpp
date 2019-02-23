// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::BeginPlay() {

	// so that first fire is after initial reload
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {

	if (RoundsLeft <= 0) {
		FiringState = EFiringState::OutOfAmmo;
	}

	else if (FPlatformTime::Seconds() - LastFireTime < ReloadTimeInSeconds) {
		FiringState = EFiringState::Reloading;
	}
	else if (IsBarrelAiming()) {

		FiringState = EFiringState::Aiming;
	}
	else {

		FiringState = EFiringState::Locked;
	}
}

void UTankAimingComponent::AimAt(FVector HitLocation) {

	if (!ensure(Barrel)) { return; }

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
		false)) {

		AimDirection = OutLaunchVelocity.GetSafeNormal();

		MoveBarrelTowards(AimDirection);

		auto time = GetWorld()->GetTimeSeconds();
		return;
	}

}

void UTankAimingComponent::Fire() {

	if (!ensure(Barrel && ProjectileBlueprint)) { return; }

	if (FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming) {
		// Spawn a projectile at the socket location
		AProjectile* Projectie = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Fire")),
			Barrel->GetSocketRotation(FName("Fire"))
			);

		Projectie->LaunchProjectile(LaunchSpeed);

		LastFireTime = FPlatformTime::Seconds();
		RoundsLeft--;
	}
}

int32 UTankAimingComponent::GetRoundsLeft() const {
	return RoundsLeft;
}

void UTankAimingComponent::Initialize(UTankBarrel * Barrel, UTankTurret * Turret) {

	this->Barrel = Barrel;
	this->Turret = Turret;
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection) {

	if (!ensure(Barrel && Turret)) { return; }

	// Work out difference between current barrel rotation and aim direction
	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotator = AimDirection.Rotation();
	FRotator DeltaRotator = AimAsRotator - BarrelRotator;
	auto time = GetWorld()->GetTimeSeconds();

	Barrel->Elevate(DeltaRotator.Pitch);
	// move the barrel the right amount

	if (FMath::Abs(DeltaRotator.Yaw) < 180 ) {

		Turret->Rotate(DeltaRotator.Yaw);
	}
	else { // avoid going the wrong way

		Turret->Rotate(-DeltaRotator.Yaw);
	}
}

bool UTankAimingComponent::IsBarrelAiming() {

	FVector BarrelForward = Barrel->GetForwardVector();

	return !(BarrelForward.Equals(AimDirection, 0.01));
}

EFiringState UTankAimingComponent::GetFiringState() {

	return FiringState;
}