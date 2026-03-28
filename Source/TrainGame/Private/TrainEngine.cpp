#include "TrainEngine.h"

ATrainEngine::ATrainEngine()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentFuel = 50.0f;
}

void ATrainEngine::BeginPlay()
{
	Super::BeginPlay();
}

void ATrainEngine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentFuel > 0.0f)
	{
		CurrentFuel -= ConsumptionRate * DeltaTime;
		CurrentFuel = FMath::Clamp(CurrentFuel, 0.0f, MaxFuel);
	}
}

void ATrainEngine::Interact_Implementation(AActor* Interactor)
{
	// Logic for adding fuel to the engine when interacted with.
	// Typically, the player would need to have a fuel item and that would be consumed.
	// For simplicity in this step, let's assume interacting adds a fixed amount if possible.
	AddFuel(10.0f);
}

FText ATrainEngine::GetInteractionName_Implementation() const
{
	return NSLOCTEXT("TrainGame", "InteractEngine", "Stoke the Engine");
}

void ATrainEngine::AddFuel(float Amount)
{
	CurrentFuel = FMath::Clamp(CurrentFuel + Amount, 0.0f, MaxFuel);
}
