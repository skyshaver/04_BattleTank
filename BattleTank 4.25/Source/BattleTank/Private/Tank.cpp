// no copy

#include "Tank.h"
#include "ConstructorHelpers.h"




ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = false;
}


void ATank::BeginPlay() 
{
	Super::BeginPlay();
	CurrentHealth = StartingHealth;
}


float ATank::GetHealthPercent() const
{
	return (float)CurrentHealth/(float)StartingHealth;
}

float ATank::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	int32 DamageToApply = FMath::Clamp(DamagePoints, 0, CurrentHealth);

	CurrentHealth -= DamageToApply;
	if (CurrentHealth <= 0) { OnDeath.Broadcast(); }
	
	return DamageToApply;
}




