// no copy

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Tank.h"
#include "Projectile.h"
#include "ConstructorHelpers.h"


UTankAimingComponent::UTankAimingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// audio
	static ConstructorHelpers::FObjectFinder<USoundCue> FireCue(TEXT("/Game/Audio/Tank_Sounds/FireCue.FireCue"));
	FireAudioCue = FireCue.Object;
	FireAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FireCue"));
	FireAudioComponent->bAutoActivate = false; // don't play the sound immediately.

	static ConstructorHelpers::FObjectFinder<USoundCue> TurretRotationCue(TEXT("/Game/Audio/Tank_Sounds/TurretRotationCue.TurretRotationCue"));
	TurretRotationAudioCue = TurretRotationCue.Object;
	TurretRotationAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("TurretRotationCue"));
	TurretRotationAudioComponent->bAutoActivate = false; // don't play the sound immediately.
}

void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	LastFireTime = FPlatformTime::Seconds(); //  all players wait for reload from start of game
}
void UTankAimingComponent::PostInitProperties()
{
	Super::PostInitProperties();
	// audio
	if (FireAudioCue->IsValidLowLevelFast())
	{
		FireAudioComponent->SetSound(FireAudioCue);
	}
	if(TurretRotationAudioCue->IsValidLowLevelFast())
	{
		TurretRotationAudioComponent->SetSound(TurretRotationAudioCue);
	}
}

void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (RoundsLeft <= 0)
	{
		FiringState = EFiringState::OutOfAmmo;
	}
	else if (FPlatformTime::Seconds() - LastFireTime < ReloadTimeInSeconds) 
	{
		FiringState = EFiringState::Reloading;
	}
	else if (bIsBarrelMoving())
	{
		FiringState = EFiringState::Aiming;
	}
	else
	{
		FiringState = EFiringState::Locked;
	}
	
}

void UTankAimingComponent::Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Turret = TurretToSet;
	Barrel = BarrelToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel || Turret)) { return; }
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
		0, // radius offset
		0, // zgravity override
		ESuggestProjVelocityTraceOption::DoNotTrace
	);
	if(bHaveAimSolution)
	{
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
		
	}
	
}

void UTankAimingComponent::MoveBarrelTowards(FVector TargetAimDirection)
{
	if (!ensure(Barrel || Turret)) { return; }
	
	// work out differnce between current barrel rotation and aim direction
	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotator = TargetAimDirection.Rotation();
	FRotator DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);
	// insure rotation always takes shortest path to crosshair
	if (FMath::Abs(DeltaRotator.Yaw) < 180)
	{
		Turret->Rotate(DeltaRotator.Yaw);
	}
	else
	{
		Turret->Rotate(-DeltaRotator.Yaw);
	}
	
	
}

bool UTankAimingComponent::bIsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }

	return !Barrel->GetForwardVector().Equals(AimDirection, .05);
}

void UTankAimingComponent::Fire()
{
	if (FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming) {
		if (!ensure(Barrel)) { return; }
		if (!ensure(ProjectileBlueprint)) { return; }
		FVector SpawnLocation = Barrel->GetSocketLocation(FName("ProjectileExit"));
		FRotator SpawnRotation = Barrel->GetSocketRotation(FName("ProjectileExit"));
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, SpawnLocation, SpawnRotation);
		Projectile->LaunchProjectile(LaunchSpeed);
		FireAudioComponent->Play();
		LastFireTime = FPlatformTime::Seconds();
		RoundsLeft--;
	}
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

int32 UTankAimingComponent::GetRoundsLeft() const
{
	return RoundsLeft;
}

// TODO need to figure out where to play TurretRotationSound from
//UE_LOG(LogTemp, Warning, TEXT("Right before audio trigger"));
//TurretRotationAudioComponent->Play();