// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankBarrel.h"
#include "Projectile.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"
#include "Engine/World.h"


// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	auto TankName = GetName();
	UE_LOG (LogTemp, Warning, TEXT("Donkey: %s C++ Constructor"), *TankName)
}

void ATank::BeginPlay() {

	Super::BeginPlay();

	auto TankName = GetName();
	UE_LOG(LogTemp, Warning, TEXT("Donkey: %s C++ Begin Play"), *TankName)
}

void ATank::AimAt(FVector HitLocation) {

	if (!ensure(TankAimingComponent)) { return; }
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed, ProjectileDebugLine);
}

void ATank::Fire() {

	if (!ensure(BarrelForTank)) { return; }
	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;

	if (BarrelForTank && isReloaded) {
		// Spawn a projectile at the socket location
		AProjectile* Projectie = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			BarrelForTank->GetSocketLocation(FName("Fire")),
			BarrelForTank->GetSocketRotation(FName("Fire"))
			);

		Projectie->LaunchProjectile(LaunchSpeed);

		LastFireTime = FPlatformTime::Seconds();
	}
}

void ATank::SetTankAimingComponent(UTankAimingComponent * TankAimingComponent) {

	this->TankAimingComponent = TankAimingComponent;
}

void ATank::SetBarrelForReference(UTankBarrel * Barrel) {

	BarrelForTank = Barrel;
}
