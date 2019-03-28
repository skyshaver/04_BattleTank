// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankTrack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	// calculate the slippage speed
	float SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
	// work-out the required acceleration this frame to correct
	FVector CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();
	// calculate and apply sideways for F = ma
	UStaticMeshComponent* TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent()); // need to cast to get access to mass in kg parameter in bp
	auto CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2; // divide by 2 as there are 2 tracks
	TankRoot->AddForce(CorrectionForce);

}

void UTankTrack::SetThrottle(float Throttle)
{
	/*float Time = GetWorld()->GetTimeSeconds();
	auto Name = GetName();
	UE_LOG(LogTemp, Warning, TEXT("%f %s Moves at : %f"), Time, *Name, Throttle);*/

	FVector ForceApplied = GetForwardVector() * Throttle * TrackMaxDrivingForce;
	FVector ForceLocation = GetComponentLocation();
	UPrimitiveComponent* TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}


