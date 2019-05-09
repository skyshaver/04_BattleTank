// no copy

#include "SpawnPoint.h"

USpawnPoint::USpawnPoint()
{

	PrimaryComponentTick.bCanEverTick = true;

}




void USpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	SpawnedActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnClass, GetComponentTransform());
	if (!SpawnedActor) { return; }
	SpawnedActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	UGameplayStatics::FinishSpawningActor(SpawnedActor, GetComponentTransform());
	
}

AActor* USpawnPoint::GetSpawnedActor() const
{
	if (!SpawnedActor) { return nullptr; }
	return SpawnedActor;
}


// Called every frame
void USpawnPoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

