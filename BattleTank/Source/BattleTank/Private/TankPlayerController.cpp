// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"

ATank* ATankPlayerController::GetControlledTank() const {

	return Cast <ATank> (GetPawn());
}

void ATankPlayerController::BeginPlay() {

	Super::BeginPlay();

	ATank* ControlledTank = GetControlledTank();

	if (!ControlledTank) {

		UE_LOG(LogTemp, Warning, TEXT("Player Controller not possesing a tank"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Player is Controlling %s"), *ControlledTank->GetName());
}
