// no copy

#include "SprungWheel.h"

// Sets default values
ASprungWheel::ASprungWheel()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringComponent = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("SpringComponent"));
	SetRootComponent(SpringComponent);
	
	MassMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("MassMesh"));
	MassMesh->SetupAttachment(SpringComponent);

	WheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("WheelMesh"));
	WheelMesh->SetupAttachment(SpringComponent);

	

}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();

	if (GetAttachParentActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("NOt null"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Null"));
	}
	
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

