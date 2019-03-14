// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	PrimaryComponentTick.bCanEverTick = true; // does this need to tick?

}


void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	Barrel = BarrelToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	
	if (!Barrel) { return; }
	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("ProjectileExit"));
	// calculate OutLaunchVelocity/ Aim Solution
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity, // OUT
		StartLocation,
		HitLocation, //whatever we're aiming at
		LaunchSpeed,
		false, // use a high arc
		0, // raidus offset
		0, // zgravity override
		ESuggestProjVelocityTraceOption::DoNotTrace

	);
	if(bHaveAimSolution)
	{
		FVector AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
		
		/* auto TankName = GetOwner()->GetName();
		 UE_LOG(LogTemp, Warning, TEXT("%s is Aiming at: %s"),*TankName, *AimDirection.ToString());*/
		/*auto time = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("Aim Solution found at %f"), time);*/
	}
	else 
	{
		auto time = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("NO Aim Solution found at %f"), time);
	}
	// UE_LOG(LogTemp, Warning, TEXT("Firing at: %f"), LaunchSpeed); // don't have to dereference primitives
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	// TODO Move barrel
	// identify actor's barrel and then use a rotator to move the barrel to the aim solution
	
	// work out differnce between current barrel rotation and aim direction
	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotator = AimDirection.Rotation();
	FRotator DeltaRotator = AimAsRotator - BarrelRotator;
	// UE_LOG(LogTemp, Warning, TEXT("Aim as Rotator: %s"), *AimAsRotator.ToString());
	

	Barrel->Elevate(DeltaRotator.Pitch);

}

