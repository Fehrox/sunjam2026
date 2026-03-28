#include "Train.h"
#include "TrainTrack.h"
#include "TrainEngine.h"
#include "Components/SplineComponent.h"

ATrain::ATrain()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATrain::BeginPlay()
{
	Super::BeginPlay();
}

void ATrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentTrack && Engine)
	{
		UpdatePositionAlongTrack(DeltaTime);
	}
}

void ATrain::UpdatePositionAlongTrack(float DeltaTime)
{
	float SpeedRatio = Engine->GetFuelRatio();
	float ActualSpeed = BaseSpeed * SpeedRatio;

	if (ActualSpeed > 0.0f)
	{
		DistanceAlongTrack += ActualSpeed * DeltaTime;

		float SplineLength = CurrentTrack->SplineComponent->GetSplineLength();

		if (DistanceAlongTrack >= SplineLength)
		{
			// Check for next track
			if (CurrentTrack->NextTracks.Num() > 0)
			{
				// In a real switch scenario, the switch would define which track is 'next'
				// For now, let's just pick the first one and reset distance.
				DistanceAlongTrack -= SplineLength;
				CurrentTrack = CurrentTrack->NextTracks[0];
			}
			else
			{
				// End of track, clamp distance.
				DistanceAlongTrack = SplineLength;
			}
		}

		if (CurrentTrack)
		{
			FVector NewLocation = CurrentTrack->SplineComponent->GetLocationAtDistanceAlongSpline(DistanceAlongTrack, ESplineCoordinateSpace::World);
			FRotator NewRotation = CurrentTrack->SplineComponent->GetRotationAtDistanceAlongSpline(DistanceAlongTrack, ESplineCoordinateSpace::World);

			SetActorLocationAndRotation(NewLocation, NewRotation);
		}
	}
}
