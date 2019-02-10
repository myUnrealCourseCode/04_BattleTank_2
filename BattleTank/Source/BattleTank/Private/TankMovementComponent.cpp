// Fill out your copyright notice in the Description page of Project Settings.


#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialize(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet) {

	if (!LeftTrackToSet || !RightTrackToSet) { return; }

	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
}

void UTankMovementComponent::RequestDirectMove(const FVector & MoveVelocity, bool bForceMaxSpeed) {

	/// no need to call Super as we are replacing the functionality
	auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	FVector AIForwardIntention = MoveVelocity.GetSafeNormal();

	float ForwardThrow = FVector::DotProduct(TankForward, AIForwardIntention);
	float RightThrow = FVector::CrossProduct(TankForward, AIForwardIntention).Z;

	IntendMoveForward(ForwardThrow);
	IntendTurnRight(RightThrow);
}

void UTankMovementComponent::IntendMoveForward(float Throw) {

	if (!LeftTrack || !RightTrack) { return; }

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw) {

	if (!LeftTrack || !RightTrack) { return; }

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}