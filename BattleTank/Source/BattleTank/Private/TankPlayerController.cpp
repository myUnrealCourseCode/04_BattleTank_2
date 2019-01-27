// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Tank.h"
#include "Engine/World.h"



void ATankPlayerController::BeginPlay() {

	Super::BeginPlay();

	ATank* ControlledTank = GetControlledTank();

	if (!ControlledTank) {

		UE_LOG(LogTemp, Warning, TEXT("Player Controller not possesing a tank"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Player is Controlling %s"), *ControlledTank->GetName());
}

// Tick
void ATankPlayerController::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	AimTowardsCrosshair();
	//UE_LOG(LogTemp, Warning, TEXT("Player Controller Ticking"));
}

ATank* ATankPlayerController::GetControlledTank() const { return Cast <ATank>(GetPawn()); }

void ATankPlayerController::AimTowardsCrosshair() {

	if (!GetControlledTank()) { return; }

	FVector OutHitLocation;

	if (GetSightRayHitLocation(OutHitLocation)) {

		GetControlledTank()->AimAt(OutHitLocation);
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