#include "Train/TrainCar.h"
#include "Train/TrainResource.h"
#include "Train/TrainResourceData.h"
#include "Train/TrainCarData.h"
#include "World/TrainTrack.h"
#include "Components/SplineComponent.h"
#include "GameFramework/TrainGameCharacter.h"

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

	FVector NewLocation;
	FRotator NewRotation;

	if (NewDistance < 0.0f)
	{
		FVector StartLocation = NewTrack->SplineComponent->GetLocationAtDistanceAlongSpline(0.0f, ESplineCoordinateSpace::World);
		FVector StartDirection = NewTrack->SplineComponent->GetDirectionAtDistanceAlongSpline(0.0f, ESplineCoordinateSpace::World);

		NewLocation = StartLocation + (StartDirection * NewDistance);
		NewRotation = StartDirection.Rotation();
	}
	else
	{
		NewLocation = NewTrack->SplineComponent->GetLocationAtDistanceAlongSpline(NewDistance, ESplineCoordinateSpace::World);
		NewRotation = NewTrack->SplineComponent->GetRotationAtDistanceAlongSpline(NewDistance, ESplineCoordinateSpace::World);
	}

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

void ATrainCar::Interact_Implementation(AActor* Interactor)
{
	if (StoredResources.Num() == 0)
	{
		return;
	}

	ATrainGameCharacter* Character = Cast<ATrainGameCharacter>(Interactor);
	if (!Character || Character->GetHeldResource() != nullptr)
	{
		return;
	}

	// Take the last resource
	TSoftObjectPtr<UTrainResourceData> ResourceData = StoredResources.Pop();

	// Spawn the resource
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Character;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = Character->GetActorLocation();
	ATrainResource* NewResource = GetWorld()->SpawnActor<ATrainResource>(ATrainResource::StaticClass(), SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	
	if (NewResource)
	{
		NewResource->ResourceData = ResourceData;
		NewResource->UpdateMeshFromData();
		// Trigger pickup immediately
		IInteractable::Execute_Interact(NewResource, Character);
	}
}

FText ATrainCar::GetInteractionName_Implementation() const
{
	if (StoredResources.Num() > 0)
	{
		return NSLOCTEXT("TrainGame", "InteractTrainCarTake", "Take Resource");
	}
	return NSLOCTEXT("TrainGame", "InteractTrainCarEmpty", "Train Car Empty");
}
