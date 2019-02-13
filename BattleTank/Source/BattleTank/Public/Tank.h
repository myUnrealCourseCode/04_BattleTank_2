// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class UTankBarrel;
class UTankTurret;
class UTankAimingComponent;
class AProjectile;
class UTankMovementComponent;

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

	virtual void BeginPlay() override;

	void AimAt(FVector OutHitLocation);

	UFUNCTION(BlueprintCallable, Category= "Setup")
	void Fire();

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSeconds = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 4000;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	bool ProjectileDebugLine = false; // Show Debug line for projectile suggestion?

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint = nullptr; // There is a an alternative called TSubclassOf<>()

	UTankBarrel* BarrelForTank = nullptr; // TODO remove

	double LastFireTime = 0;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetTankAimingComponent(UTankAimingComponent* TankAimingComponent);

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetBarrelForReference(UTankBarrel* Barrel);

protected:

	UPROPERTY(BlueprintReadOnly)
	UTankAimingComponent* TankAimingComponent = nullptr;
};