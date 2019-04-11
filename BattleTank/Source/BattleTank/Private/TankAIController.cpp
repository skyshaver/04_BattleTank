// Fill out your copyright notice in the Description page of Project Settings.
#include "TankAIController.h"
#include "TankMovementComponent.h"
#include "TankAimingComponent.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();	// insures we're overriding beginPlay from the super(parent) class

}

void ATankAIController::Tick(float DeltaTime)
{
	APawn* PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	APawn* ControlledTank = GetPawn();

	Super::Tick(DeltaTime);

	if (!ensure(PlayerTank || ControlledTank)) { return; }
	
	// move towards player
	MoveToActor(PlayerTank, AcceptanceRadius);

	// aim at player and fire
	UTankAimingComponent* AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }
	
	AimingComponent->AimAt(PlayerTank->GetActorLocation());
	
	if (AimingComponent->GetFiringState() == EFiringState::Locked)
	{
		AimingComponent->Fire();
	}
	
	

}



