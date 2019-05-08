// no copy

#include "SprungWheel.h"

// Sets default values
ASprungWheel::ASprungWheel()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringConstraintComponent = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("SpringComponent"));
	SetRootComponent(SpringConstraintComponent);
	
	WheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("WheelMesh"));
	WheelMesh->SetupAttachment(SpringConstraintComponent);

	

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
	SpringConstraintComponent->SetConstrainedComponents(BodyRoot, NAME_None, WheelMesh, NAME_None);
}


void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

