#include "TrainCar.h"
#include "TrainResource.h"
#include "TrainResourceData.h"
#include "Components/StaticMeshComponent.h"

ATrainCar::ATrainCar()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

void ATrainCar::BeginPlay()
{
	Super::BeginPlay();
}

bool ATrainCar::TryStoreResource(ATrainResource* Resource)
{
	if (!Resource || !Resource->ResourceData)
	{
		return false;
	}

	UTrainResourceData* Data = Resource->ResourceData.LoadSynchronous();
	if (CanStoreResource(Data))
	{
		StoredResources.Add(Resource->ResourceData);
		Resource->Destroy();
		return true;
	}

	return false;
}

bool ATrainCar::CanStoreResource(UTrainResourceData* ResourceData) const
{
	if (!ResourceData || StoredResources.Num() >= MaxCapacity)
	{
		return false;
	}

	// If AcceptedResources is empty, we accept any resource
	if (AcceptedResources.Num() == 0)
	{
		return true;
	}

	for (const TSoftObjectPtr<UTrainResourceData>& Accepted : AcceptedResources)
	{
		if (Accepted.ToSoftObjectPath() == FSoftObjectPath(ResourceData))
		{
			return true;
		}
	}

	return false;
}
