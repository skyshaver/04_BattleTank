// no copy

#include "SprungWheel.h"

// Sets default values
ASprungWheel::ASprungWheel()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringConstraintComponent = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("SpringConstraintComponent"));
	SetRootComponent(SpringConstraintComponent);

	AxleMesh = CreateDefaultSubobject<USphereComponent>(FName("AxleMesh"));
	AxleMesh->SetupAttachment(SpringConstraintComponent);

	WheelMesh = CreateDefaultSubobject<USphereComponent>(FName("WheelMesh"));
	WheelMesh->SetupAttachment(AxleMesh);

	AxleConstraintComponent = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("AxleConstraintComponent"));
	AxleConstraintComponent->SetupAttachment(AxleMesh);
}

// Called when the game starts or when spawned
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
	SpringConstraintComponent->SetConstrainedComponents(BodyRoot, NAME_None, AxleMesh, NAME_None);
	AxleConstraintComponent->SetConstrainedComponents(AxleMesh, NAME_None, WheelMesh, NAME_None);
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

