#include "TrainEngine.h"

UTrainEngineComponent::UTrainEngineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentFuel = 50.0f;
}

void UTrainEngineComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTrainEngineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentFuel > 0.0f)
	{
		CurrentFuel -= ConsumptionRate * DeltaTime;
		CurrentFuel = FMath::Clamp(CurrentFuel, 0.0f, MaxFuel);
		OnFuelChange.Broadcast(CurrentFuel, MaxFuel);
	}
}

void UTrainEngineComponent::Interact_Implementation(AActor* Interactor)
{
	// Logic for adding fuel to the engine when interacted with.
	// Typically, the player would need to have a fuel item and that would be consumed.
	// For simplicity in this step, let's assume interacting adds a fixed amount if possible.
	AddFuel(10.0f);
}

FText UTrainEngineComponent::GetInteractionName_Implementation() const
{
	return NSLOCTEXT("TrainGame", "InteractEngine", "Stoke the Engine");
}

void UTrainEngineComponent::AddFuel(float Amount)
{
	CurrentFuel = FMath::Clamp(CurrentFuel + Amount, 0.0f, MaxFuel);

	OnFuelAdded.Broadcast(CurrentFuel, MaxFuel);
}
