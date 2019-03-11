// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();	// insaures we're overriding beginPlay from the super(parent) class

	ATank* ControlledTank = GetControlledTank();

	if (!ControlledTank) { UE_LOG(LogTemp, Warning, TEXT("Failed to capture pawn")); }
	else { UE_LOG(LogTemp, Warning, TEXT("Pawn %s is under Player control"), *ControlledTank->GetName()); }

	// UE_LOG(LogTemp, Warning, TEXT("Player Controller Begin Play"));
}

ATank*  ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

