// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"

ATank* ATankAIController::GetControlledTank() const { return Cast<ATank> (GetPawn()); }

ATank* ATankAIController::GetPlayerTank() const {

	ATank* PlayerTank = Cast<ATank> (GetWorld()->GetFirstPlayerController()->GetPawn());

	if (!PlayerTank) { return nullptr; }

	return PlayerTank;
}

void ATankAIController::BeginPlay() {

	Super::BeginPlay();

	//ATank* ControlledTank = GetControlledTank();

	ATank* PlayerTank = GetPlayerTank();

	if (!PlayerTank) {

		UE_LOG(LogTemp, Warning, TEXT("AI could not find player tank"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("AI found player tank: %s"), *PlayerTank->GetName());
}

void ATankAIController::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	if (GetPlayerTank()) {

		// TODO move toward the player

		// Aim toward the player
		GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());
		// Fire if ready 
	}
}
