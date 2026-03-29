#include "Train.h"
#include "TrainTrack.h"
#include "TrainEngine.h"
#include "TrainCar.h"
#include "TrainCarData.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Components/SplineComponent.h"

ATrain::ATrain()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Engine = CreateDefaultSubobject<UTrainEngineComponent>(TEXT("Engine"));
	Engine->SetupAttachment(RootComponent);
}

void ATrain::BeginPlay()
{
	Super::BeginPlay();
	if (CurrentTrack)
	{
		TrackHistory.Add(CurrentTrack);
	}
	SpawnTrainCars();
}

void ATrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentTrack && Engine)
	{
		UpdatePositionAlongTrack(DeltaTime);
	}
}

void ATrain::InitialiseTrain(ATrainTrack* StartingTrack) {
	if (StartingTrack != nullptr) {
		CurrentTrack = StartingTrack;
		TrackHistory.Empty();
		TrackHistory.Add(CurrentTrack);
	}
}

void ATrain::UpdatePositionAlongTrack(float DeltaTime)
{
	if (bIsDerailed) return;

	float SpeedRatio = Engine->GetFuelRatio();
	float ActualSpeed = BaseSpeed * SpeedRatio;

	if (SpeedCurve)
	{
		ActualSpeed = SpeedCurve->GetFloatValue(SpeedRatio) * BaseSpeed;
	}

	if (ActualSpeed > 0.0f)
	{
		DistanceAlongTrack += ActualSpeed * DeltaTime;

		while (CurrentTrack && DistanceAlongTrack >= CurrentTrack->SplineComponent->GetSplineLength())
		{
			float SplineLength = CurrentTrack->SplineComponent->GetSplineLength();
			
			if (CurrentTrack->NextTracks.Num() > 0)
			{
				DistanceAlongTrack -= SplineLength;
				CurrentTrack = CurrentTrack->NextTracks[0];

				// Add to history
				TrackHistory.Add(CurrentTrack);
				if (TrackHistory.Num() > MaxTrackHistory)
				{
					TrackHistory.RemoveAt(0);
				}
			}
			else
			{
				DistanceAlongTrack = SplineLength;
				break;
			}
		}

		if (CurrentTrack)
		{
			FVector NewLocation = CurrentTrack->SplineComponent->GetLocationAtDistanceAlongSpline(DistanceAlongTrack, ESplineCoordinateSpace::World);
			FRotator NewRotation = CurrentTrack->SplineComponent->GetRotationAtDistanceAlongSpline(DistanceAlongTrack, ESplineCoordinateSpace::World);

			SetActorLocationAndRotation(NewLocation, NewRotation);
		}
	}

	// Update cars
	for (int32 i = 0; i < CarInstances.Num(); ++i)
	{
		float CarTargetDistance = DistanceAlongTrack - (CarDistance * (i + 1));
		int32 HistoryIndex = TrackHistory.Num() - 1;
		ATrainTrack* CarTrack = CurrentTrack;

		while (CarTargetDistance < 0.0f && HistoryIndex > 0)
		{
			// Go back in history instead of using PreviousTracks
			HistoryIndex--;
			CarTrack = TrackHistory[HistoryIndex];
			
			if (CarTrack && CarTrack->SplineComponent)
			{
				CarTargetDistance += CarTrack->SplineComponent->GetSplineLength();
			}
			else
			{
				break;
			}
		}

		if (CarTrack)
		{
			CarInstances[i]->UpdatePosition(CarTrack, FMath::Max(0.0f, CarTargetDistance));
		}
	}
}

void ATrain::Derail()
{
	if (bIsDerailed) return;

	bIsDerailed = true;
	OnDerailed.Broadcast();
}

void ATrain::SpawnTrainCars()
{
	if (!GetWorld()) return;

	for (int32 i = 0; i < TrainCarsToSpawn.Num(); ++i)
	{
		if (UTrainCarData* CarData = TrainCarsToSpawn[i])
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ATrainCar* NewCar = GetWorld()->SpawnActor<ATrainCar>(TrainCarClass, GetActorTransform(), SpawnParams);
			if (NewCar)
			{
				float InitialCarDistance = DistanceAlongTrack - (CarDistance * (i + 1));
				NewCar->Initialise(CarData, CurrentTrack, InitialCarDistance);
				CarInstances.Add(NewCar);
			}
		}
	}
}
