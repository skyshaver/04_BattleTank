// no copy

#include "TankMovementComponent.h"
#include "TankTrack.h"

void  UTankMovementComponent::Initialize(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	FVector TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal(); // direction tank is facing
	FVector AIForwawdIntention = MoveVelocity.GetSafeNormal();
	
	// dot product
	float ForwardThrow = FVector::DotProduct(TankForward, AIForwawdIntention);
	IntendMoveForward(ForwardThrow);

	// cross product
	float RightThrow = FVector::CrossProduct(TankForward, AIForwawdIntention).Z;
	IntendTurnRight(RightThrow);

}

void  UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (!LeftTrack || !RightTrack) { return; }
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
}

void  UTankMovementComponent::IntendTurnRight(float Throw)
{
	if (!LeftTrack || !RightTrack) { return; }
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}

