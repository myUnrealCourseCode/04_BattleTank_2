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

	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming Component"));
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATank::AimAt(FVector HitLocation) {

	TankAimingComponent->AimAt(HitLocation, LaunchSpeed, ProjectileDebugLine);
}

void ATank::Fire() {

	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;

	if (Barrel && isReloaded) {
		// Spawn a projectile at the socket location
		AProjectile* Projectie = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Fire")),
			Barrel->GetSocketRotation(FName("Fire"))
			);

		Projectie->LaunchProjectile(LaunchSpeed);

		LastFireTime = FPlatformTime::Seconds();
	}
}

void ATank::SetBarrelReference(UTankBarrel * BarrelToSet) {

	TankAimingComponent->SetBarrelReference(BarrelToSet);
	Barrel = BarrelToSet;
}

void ATank::SetTurretReference(UTankTurret * TurretToSet) {

	TankAimingComponent->SetTurretReference(TurretToSet);
}
