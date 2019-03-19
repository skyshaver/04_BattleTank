// Fill out your copyright notice in the Description page of Project Settings.
#include "TankAIController.h"
#include "Tank.h"

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
		// TODO move towards player
		// aim at player
		ControlledTank->AimAt(PlayerTank->GetActorLocation());
		// fire when ready
		ControlledTank->Fire(); // TODO don't fire every frame
	}

}



