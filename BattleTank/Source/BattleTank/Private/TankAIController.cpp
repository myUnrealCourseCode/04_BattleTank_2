// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Engine/World.h"


void ATankAIController::BeginPlay() {

	Super::BeginPlay();

}

void ATankAIController::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	APawn* ControlledTank = GetPawn();

	APawn* PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!ensure(PlayerTank && ControlledTank)) {return; }

	// move toward the player
	MoveToActor(PlayerTank, AcceptanceRadius); // TODO check radius is in cm

	// Aim toward the player
	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	AimingComponent->Fire(); // TODO limit firing rate
}
