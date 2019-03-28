// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay()
{
	Super::BeginPlay();
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);

}


void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// drive tracks, apply sideways force
	DriveTrack();
	ApplySidewaysForce();
	CurrentThrottle = 0;

	
}

void UTankTrack::ApplySidewaysForce()
{
	// calculate the slippage speed
	float SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
	// work-out the required acceleration this frame to correct
	FVector CorrectionAcceleration = -SlippageSpeed / GetWorld()->GetDeltaSeconds() * GetRightVector();
	// calculate and apply sideways for F = ma
	UStaticMeshComponent* TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent()); // need to cast to get access to mass in kg parameter in bp
	auto CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2; // divide by 2 as there are 2 tracks
	TankRoot->AddForce(CorrectionForce);
}
void UTankTrack::SetThrottle(float Throttle)
{
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1, 1);
}

void UTankTrack::DriveTrack()
{
	FVector ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
	FVector ForceLocation = GetComponentLocation();
	UPrimitiveComponent* TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}


