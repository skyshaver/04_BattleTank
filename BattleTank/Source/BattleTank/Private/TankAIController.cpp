// Fill out your copyright notice in the Description page of Project Settings.
#include "TankAIController.h"
#include "Tank.h"
#include "TankMovementComponent.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();	// insures we're overriding beginPlay from the super(parent) class
}

void ATankAIController::Tick(float DeltaTime)
{
	PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	ControlledTank = Cast<ATank>(GetPawn());

	Super::Tick(DeltaTime);

	if (PlayerTank)
	{
		// move towards player
		MoveToActor(PlayerTank, AcceptanceRadius);
		
		// aim at player
		ControlledTank->AimAt(PlayerTank->GetActorLocation());
		// fire when ready
		ControlledTank->Fire(); 
	}

}



