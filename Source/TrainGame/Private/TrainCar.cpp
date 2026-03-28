#include "TrainCar.h"
#include "TrainResource.h"
#include "TrainResourceData.h"
#include "TrainCarData.h"
#include "TrainTrack.h"
#include "Components/SplineComponent.h"

ATrainCar::ATrainCar()
{
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;
}

void ATrainCar::BeginPlay()
{
	Super::BeginPlay();
}

void ATrainCar::Initialise_Implementation(UTrainCarData* NewCarData, ATrainTrack* StartingTrack, float InitialDistance) {
	if (!NewCarData) return;

	this->CarData = NewCarData;
	
	AcceptedResources = CarData->AcceptedResources;
	MaxCapacity = CarData->MaxCapacity;

	UpdatePosition(StartingTrack, InitialDistance);
}

void ATrainCar::UpdatePosition(ATrainTrack* NewTrack, float NewDistance)
{
	if (!NewTrack || !NewTrack->SplineComponent) return;

	FVector NewLocation = NewTrack->SplineComponent->GetLocationAtDistanceAlongSpline(NewDistance, ESplineCoordinateSpace::World);
	FRotator NewRotation = NewTrack->SplineComponent->GetRotationAtDistanceAlongSpline(NewDistance, ESplineCoordinateSpace::World);

	SetActorLocationAndRotation(NewLocation, NewRotation);
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
