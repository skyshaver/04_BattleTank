// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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
		LaunchSpeed, // 
		ESuggestProjVelocityTraceOption::DoNotTrace

	);
	if(bHaveAimSolution)
	{
		auto AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
		// auto TankName = GetOwner()->GetName();
		// UE_LOG(LogTemp, Warning, TEXT("%s is Aiming at: %s"),*TankName, *AimDirection.ToString());
		
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
	

	Barrel->Elevate(5);

}

