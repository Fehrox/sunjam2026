#include "TrainEngine.h"
#include "TrainResource.h"
#include "TrainResourceData.h"

UTrainEngineComponent::UTrainEngineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	//SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	//SetGenerateOverlapEvents(true);
	//SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));

	OnComponentHit.AddDynamic(this, &UTrainEngineComponent::OnHit);
	OnComponentBeginOverlap.AddDynamic(this, &UTrainEngineComponent::OnOverlapBegin);
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

void UTrainEngineComponent::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit) {
	if (ATrainResource* Resource = Cast<ATrainResource>(OtherActor))
	{
		UTrainResourceData* Data = Resource->ResourceData.LoadSynchronous();
		if (Data && Data->FuelValue > 0)
		{
			AddFuel(Data->FuelValue);
			Resource->Destroy();
		}
	}
}

void UTrainEngineComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (ATrainResource* Resource = Cast<ATrainResource>(OtherActor))
	{
		UTrainResourceData* Data = Resource->ResourceData.LoadSynchronous();
		if (Data && Data->FuelValue > 0)
		{
			AddFuel(Data->FuelValue);
			Resource->Destroy();
		}
	}
}
