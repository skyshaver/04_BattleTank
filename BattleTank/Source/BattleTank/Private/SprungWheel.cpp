// no copy

#include "SprungWheel.h"

// Sets default values
ASprungWheel::ASprungWheel()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;

	SpringConstraintComponent = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("SpringComponent"));
	SetRootComponent(SpringConstraintComponent);
	
	AxelMesh = CreateDefaultSubobject<USphereComponent>(FName("AxelMesh"));
	AxelMesh->SetupAttachment(SpringConstraintComponent);

	WheelMesh = CreateDefaultSubobject<USphereComponent>(FName("WheelMesh"));
	WheelMesh->SetupAttachment(AxelMesh);

	AxelConstraintComponent = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("AxelComponent"));
	AxelConstraintComponent->SetupAttachment(AxelMesh);

}



void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();

	WheelMesh->SetNotifyRigidBodyCollision(true);
	WheelMesh->OnComponentHit.AddDynamic(this, &ASprungWheel::OnHit);
	SetupConstraint();
	
}

void ASprungWheel::SetupConstraint()
{
	if (!GetAttachParentActor()) { return; }
	UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());
	if (!BodyRoot) { return; }
	SpringConstraintComponent->SetConstrainedComponents(BodyRoot, NAME_None, AxelMesh, NAME_None);
	AxelConstraintComponent->SetConstrainedComponents(AxelMesh, NAME_None, WheelMesh, NAME_None);
}


void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TickGroup == TG_PostPhysics)
	{
		TotalForceMagnitudeThisFrame = 0;
	}
	

}

void ASprungWheel::AddDrivingForce(float ForceMagnitude)
{
	TotalForceMagnitudeThisFrame += ForceMagnitude;
}

void ASprungWheel::ApplyForce()
{
	WheelMesh->AddForce(AxelMesh->GetForwardVector() * TotalForceMagnitudeThisFrame);
}


void ASprungWheel::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	ApplyForce();
}
