#include "World/TrainTrackSwitch.h"
#include "World/TrainTrack.h"

ATrainTrackSwitch::ATrainTrackSwitch()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATrainTrackSwitch::BeginPlay()
{
	Super::BeginPlay();

	ToggleSwitch();
}

void ATrainTrackSwitch::Interact_Implementation(AActor* Interactor)
{
	if (TargetTracks.Num() > 0)
	{
		CurrentTargetIndex = (CurrentTargetIndex + 1) % TargetTracks.Num();
		ToggleSwitch();
		OnSwitchToggled.Broadcast(CurrentTargetIndex);
	}
}

FText ATrainTrackSwitch::GetInteractionName_Implementation() const
{
	return NSLOCTEXT("TrainGame", "InteractSwitch", "Toggle Switch");
}

void ATrainTrackSwitch::ToggleSwitch()
{
	if (SourceTrack && CurrentTargetIndex < TargetTracks.Num())
	{
		SourceTrack->NextTracks.Empty();
		SourceTrack->NextTracks.Add(TargetTracks[CurrentTargetIndex]);
	}
}
