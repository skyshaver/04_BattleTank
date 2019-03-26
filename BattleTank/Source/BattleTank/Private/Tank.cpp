// no copy

#include "Tank.h"
#include "TankBarrel.h"
#include "Projectile.h"

ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = false;
}
void ATank::BeginPlay() 
{
	Super::BeginPlay();
	
}

void ATank::Fire()
{
	if (!ensure(Barrel)) { return; }
	bool IsReloaded = ((FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds);
	if (IsReloaded) {

		FVector SpawnLocation = Barrel->GetSocketLocation(FName("ProjectileExit"));
		FRotator SpawnRotation = Barrel->GetSocketRotation(FName("ProjectileExit"));
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, SpawnLocation, SpawnRotation);
		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
}



