#include "TrainResource.h"
#include "TrainResourceData.h"
#include "Components/StaticMeshComponent.h"

ATrainResource::ATrainResource()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

void ATrainResource::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UpdateMeshFromData();
}

void ATrainResource::BeginPlay()
{
	Super::BeginPlay();
	UpdateMeshFromData();
}

void ATrainResource::UpdateMeshFromData()
{
	if (ResourceData.ToSoftObjectPath().IsValid())
	{
		UTrainResourceData* ResourceDataHardPtr = ResourceData.LoadSynchronous();
		if (ResourceDataHardPtr && ResourceDataHardPtr->DisplayMesh)
		{
			MeshComponent->SetStaticMesh(ResourceDataHardPtr->DisplayMesh);
		}
	}
}

void ATrainResource::Interact_Implementation(AActor* Interactor)
{
	OnResourcePickedUp(Interactor);
}

FText ATrainResource::GetInteractionName_Implementation() const
{
	if (ResourceData)
	{
		return FText::Format(NSLOCTEXT("TrainGame", "InteractResource", "Pick up {0}"), ResourceData->ResourceName);
	}
	return NSLOCTEXT("TrainGame", "InteractResourceDefault", "Pick up resource");
}

void ATrainResource::OnResourcePickedUp(AActor* Interactor)
{
	// Logic for picking up the resource and attaching it to the player.
	// For now, let's just destroy the actor and assume the player "has" it.
	// In a real implementation, we'd probably call a function on the Interactor (e.g., ICarryInterface::PickUpItem)
	Destroy();
}
