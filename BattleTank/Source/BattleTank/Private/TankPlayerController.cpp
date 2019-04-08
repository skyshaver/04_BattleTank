// Fill out your copyright notice in the Description page of Project Settings.
#include "TankPlayerController.h"

#include "TankAimingComponent.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UTankAimingComponent* AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }
	FoundAimingComponent(AimingComponent); // broadcasts reference up to bp
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );

	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetPawn()) { return; }
	UTankAimingComponent* AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }
	FVector HitLocation; // OUT parameter
	
	if (GetSightRayHitLocation(HitLocation))
	{
		AimingComponent->AimAt(HitLocation);
	}

	
}

// Get world location of linetrace through crosshair
bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	// Find the crosshair position
	int32 ViewPortSizeX, ViewPortSizeY;
	FVector LookDirection;
	GetViewportSize(ViewPortSizeX, ViewPortSizeY);  // uses OUT parameters of Get...() to init ViewPort x,y
	FVector2D ScreenLocation = FVector2D(ViewPortSizeX * CrossHairXLocation, ViewPortSizeY * CrossHairYLocation);
	
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		return GetLookVectorHitLocation( LookDirection, HitLocation);
		// UE_LOG(LogTemp, Warning, TEXT("HitLocation: %s"), *HitLocation.ToString()); 
		
	}
	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	// "De-project" the screen position of the crosshair to a world direction
	
	FVector WorldLocation; // to be discarded
	float ScreenX = ScreenLocation.X;
	float ScreenY = ScreenLocation.Y;
	return DeprojectScreenPositionToWorld(ScreenX, ScreenY, WorldLocation, LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;
	FVector StartLocation = PlayerCameraManager->GetCameraLocation();
	FVector EndLocation = StartLocation + (LookDirection * LineTraceRange);
	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility
	))
	{
		HitLocation = HitResult.Location;
		return true;
	}
	else
	{
		HitLocation = FVector(0);
		return false;
	}
	
}





