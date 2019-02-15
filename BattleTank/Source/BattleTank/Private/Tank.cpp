// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankBarrel.h"
#include "Projectile.h"
#include "Engine/World.h"


// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	auto TankName = GetName();
}

void ATank::BeginPlay() {

	Super::BeginPlay();

	auto TankName = GetName();
}

void ATank::Fire() {

	if (!ensure(BarrelForTank)) { return; }
	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;

	if (isReloaded) {
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

void ATank::SetBarrelForReference(UTankBarrel * Barrel) {

	BarrelForTank = Barrel;
}
