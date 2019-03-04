// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "TankAimingComponent.h"
#include "Engine/World.h"
#include "Tank.h"



void ATankPlayerController::BeginPlay() {

	Super::BeginPlay();
	if (!(GetPawn())) { return; }
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();

	if (!ensure(AimingComponent)) { return; }
	FoundAimingComponent(AimingComponent);
}

// Tick
void ATankPlayerController::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);
	
	if (!(GetPawn())) { return; }

	AimTowardsCrosshair();
	//UE_LOG(LogTemp, Warning, TEXT("Player Controller Ticking"));
}

void ATankPlayerController::SetPawn(APawn * InPawn) {

	Super::SetPawn(InPawn);

	if (InPawn) {

		auto OurTank = Cast <ATank>(InPawn);
		if (!ensure(OurTank)) { return; }
		OurTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossessedTankDeath);
	}
}

void ATankPlayerController::OnPossessedTankDeath() {

	StartSpectatingOnly();
}

void ATankPlayerController::AimTowardsCrosshair() {

	if (!(GetPawn())) { return; }
	if (!ensure(GetPawn())) { return; }

	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }

	FVector OutHitLocation;

	bool  bHitLocation = GetSightRayHitLocation(OutHitLocation);
	//UE_LOG(LogTemp, Warning, TEXT("HitLocation: %i"), bHitLocation);
	if (bHitLocation) {

		AimingComponent->AimAt(OutHitLocation);
	}
}

// get world location of linetrace through crosshair
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const{

	// find crosshair position in pixel coordinates
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	FVector2D ScreenLocation (ViewportSizeX * CrossHairXLocation, ViewportSizeY * CrossHairYLocation);

	// De-project the screen position to world
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection)) {

		return GetLookVectorHitLocation(LookDirection, OutHitLocation);
	}
	
	return false;
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& OutHitLocation) const{

	FHitResult OutHitResult;
	FVector StartLocation = PlayerCameraManager->GetCameraLocation();
	FVector EndLocation = LookDirection * LineTraceRange;

	if (GetWorld()->LineTraceSingleByChannel(
		OutHitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility)) {

		OutHitLocation = OutHitResult.Location;
		return true;
	}

	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const {

	FVector CameraWorldLocation; // to be discarded

	return DeprojectScreenPositionToWorld(ScreenLocation.X,
		ScreenLocation.Y,
		CameraWorldLocation,
		LookDirection );
}