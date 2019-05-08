// no copy

#include "SprungWheel.h"

// Sets default values
ASprungWheel::ASprungWheel()
{
	PrimaryActorTick.bCanEverTick = true;

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

}

