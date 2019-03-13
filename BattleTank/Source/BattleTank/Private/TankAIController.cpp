// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();	// insures we're overriding beginPlay from the super(parent) class

	ATank* ControlledTank = GetControlledTank();
	if (!ControlledTank) { UE_LOG(LogTemp, Warning, TEXT("Failed to capture pawn")); }
	else { UE_LOG(LogTemp, Warning, TEXT("Pawn %s is under AI control"), *ControlledTank->GetName()); }

	ATank* PlayerTank = GetPlayerTank();
	if (!PlayerTank) { UE_LOG(LogTemp, Warning, TEXT("Failed to locate PlayerTank")); }
	else { UE_LOG(LogTemp, Warning, TEXT("Player Controller %s is found by AI controller"), *PlayerTank->GetName()); }

}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetPlayerTank())
	{
		// TODO move towards player
		// aim at player
		GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());
		// TODO fire when ready
	}

}

ATank*  ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank * ATankAIController::GetPlayerTank() const
{
	return Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

