// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "ConstructorHelpers.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("CollisionMesh"));
	SetRootComponent(CollisionMesh);
	CollisionMesh->SetNotifyRigidBodyCollision(true);
	CollisionMesh->SetVisibility(false);

	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("LaunchBlast"));
	LaunchBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("ImpactBlast"));
	ImpactBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlast->bAutoActivate = false;

	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("ExplosionForce"));
	ExplosionForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	ProjectileMovement->bAutoActivate = false;

	// audio
	static ConstructorHelpers::FObjectFinder<USoundCue> ProjectileImpactCue(TEXT("/Game/Audio/Tank_Sounds/ProjectileImpactCue.ProjectileImpactCue"));
	// Store a reference to the Cue asset - we'll need it later.
	ProjectileAudioCue = ProjectileImpactCue.Object;
	// Create an audio component, the audio component wraps the Cue, and allows us to interact with
	// it, and its parameters from code.
	ProjectileAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ProjectileImpactCue"));
	// I don't want the sound playing the moment it's created.
	ProjectileAudioComponent->bAutoActivate = false; // don't play the sound immediately.
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	
}

void AProjectile::PostInitProperties()
{
	Super::PostInitProperties();
	if (ProjectileAudioCue->IsValidLowLevelFast())
	{
		ProjectileAudioComponent->SetSound(ProjectileAudioCue);
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	LaunchBlast->Deactivate();
	ImpactBlast->Activate();
	ExplosionForce->FireImpulse();

	// destroys mesh but does not destroy underlying object
	SetRootComponent(ImpactBlast);
	CollisionMesh->DestroyComponent();

	// damage to actor
	UGameplayStatics::ApplyRadialDamage(
		this,
		BaseDamage,
		GetActorLocation(),
		ExplosionForce->Radius,
		UDamageType::StaticClass(),
		TArray<AActor*>() //Damage all actors in radius
	);
	

	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());

	/*UGameplayStatics::ApplyDamage(
		OtherActor,
		BaseDamage,
		nullptr,
		this,
		UDamageType::StaticClass()
	
	);*/

	ProjectileAudioComponent->Play();

	// set timer from creation of object which deletes object from memory after 5 secs.
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&AProjectile::OnTimerExpire, // out parameter that calls local delegate method
		DestroyDelay,
		false
		
	);
}
void AProjectile::OnTimerExpire()
{
	Destroy();
}

void AProjectile::LaunchProjectile(float LaunchSpeed)
{
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * LaunchSpeed);
	ProjectileMovement->Activate();
}
