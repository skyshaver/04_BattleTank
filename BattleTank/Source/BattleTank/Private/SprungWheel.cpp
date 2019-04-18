// no copy

#include "SprungWheel.h"

// Sets default values
ASprungWheel::ASprungWheel()
{
	PrimaryActorTick.bCanEverTick = true;

	MassMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("MassMesh"));
	SetRootComponent(MassMesh);

	WheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("WheelMesh"));
	WheelMesh->SetupAttachment(MassMesh);

	SpringComponent = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("SpringComponent"));
	SpringComponent->SetupAttachment(MassMesh);

}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

