// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"
#include "Engine/World.h"


void ATankAIController::BeginPlay() {

	Super::BeginPlay();

}

void ATankAIController::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	ATank* ControlledTank = Cast<ATank>(GetPawn());
	ATank* PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (PlayerTank) {

		// move toward the player
		MoveToActor(PlayerTank, AcceptanceRadius); // TODO check radius is in cm

		// Aim toward the player
		ControlledTank->AimAt(PlayerTank->GetActorLocation());

		// Fire if ready
		ControlledTank->Fire(); // TODO limit firing rate
	}
}
