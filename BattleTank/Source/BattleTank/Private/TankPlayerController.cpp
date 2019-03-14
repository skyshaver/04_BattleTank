// Fill out your copyright notice in the Description page of Project Settings.
#include "TankPlayerController.h"
#include "Tank.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();	// insures we're overriding beginPlay from the super(parent) class

	ATank* ControlledTank = GetControlledTank();

	if (!ControlledTank) { UE_LOG(LogTemp, Warning, TEXT("Failed to capture pawn")); }
	else { UE_LOG(LogTemp, Warning, TEXT("Pawn %s is under Player control"), *ControlledTank->GetName()); }

}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );

	AimTowardsCrosshair();

}

void ATankPlayerController::AimTowardsCrosshair()
{
	
	if (!GetControlledTank()) { return; } // insures we possess a PC before we try to aim

	FVector HitLocation; // OUT parameter
	if (GetSightRayHitLocation(HitLocation)) 
	{
		GetControlledTank()->AimAt(HitLocation);
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
		GetLookVectorHitLocation( LookDirection, HitLocation);
		// UE_LOG(LogTemp, Warning, TEXT("HitLocation: %s"), *HitLocation.ToString()); 
		return true;
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



ATank*  ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}



