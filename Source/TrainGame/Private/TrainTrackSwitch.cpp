#include "TrainTrackSwitch.h"
#include "TrainTrack.h"

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
	CurrentTargetIndex = (CurrentTargetIndex + 1) % TargetTracks.Num();
	ToggleSwitch();
}

FText ATrainTrackSwitch::GetInteractionName_Implementation() const
{
	return NSLOCTEXT("TrainGame", "InteractSwitch", "Toggle Switch");
}

void ATrainTrackSwitch::ToggleSwitch()
{
	if (SourceTrack && TargetTracks.Num() > 0)
	{
		SourceTrack->NextTracks.Empty();
		SourceTrack->NextTracks.Add(TargetTracks[CurrentTargetIndex]);
	}
}
