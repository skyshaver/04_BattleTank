// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankBarrel.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"
#include "Projectile.h"
// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	UE_LOG(LogTemp, Warning, TEXT("Turkey: ATank %s C++ constructor called"), *GetName());
	
}
void ATank::BeginPlay() 
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Turkey: ATank %s C++ BeginPlay called"), *GetName());

}

void ATank::Fire()
{
	bool IsReloaded = ((FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds);
	if (Barrel && IsReloaded) {
		// otherwise spawn a projectile at the barrel exit socket 'ProjectileExit'

		FVector SpawnLocation = Barrel->GetSocketLocation(FName("ProjectileExit"));
		FRotator SpawnRotation = Barrel->GetSocketRotation(FName("ProjectileExit"));
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, SpawnLocation, SpawnRotation);
		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
}

void ATank::AimAt(FVector HitLocation) 
{
	if (!TankAimingComponent) { return; }

	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
	
}



