#include "Train.h"
#include "TrainTrack.h"
#include "TrainEngine.h"
#include "TrainCar.h"
#include "TrainCarData.h"
#include "Bridge.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Components/SplineComponent.h"

UTrainBrakeSwitch::UTrainBrakeSwitch()
{
	//SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	//SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	//SetGenerateOverlapEvents(true);
}

void UTrainBrakeSwitch::Interact_Implementation(AActor* Interactor)
{
	if (ATrain* Train = Cast<ATrain>(GetOwner()))
	{
		Train->ToggleBrakes();
		OnBrakeSwitchToggled.Broadcast(Train->AreBrakesEnabled());
	}
}

FText UTrainBrakeSwitch::GetInteractionName_Implementation() const
{
	if (const ATrain* Train = Cast<ATrain>(GetOwner()))
	{
		return Train->AreBrakesEnabled() ? FText::FromString("Release Brakes") : FText::FromString("Apply Brakes");
	}
	return FText::FromString("Brake Switch");
}

ATrain::ATrain()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Engine = CreateDefaultSubobject<UTrainEngineComponent>(TEXT("Engine"));
	Engine->SetupAttachment(RootComponent);

	BrakeSwitch = CreateDefaultSubobject<UTrainBrakeSwitch>(TEXT("BrakeSwitch"));
	BrakeSwitch->SetupAttachment(RootComponent);
}

void ATrain::BeginPlay()
{
	Super::BeginPlay();

	if (Engine)
	{
		Engine->OnComponentHit.AddDynamic(this, &ATrain::OnEngineHit);
		Engine->OnComponentBeginOverlap.AddDynamic(this, &ATrain::OnEngineOverlap);
	}

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

void ATrain::ToggleBrakes()
{
	bBrakesEnabled = !bBrakesEnabled;
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

	// Apply brakes
	if (bBrakesEnabled)
	{
		BrakeProgress = FMath::Clamp(BrakeProgress + (DeltaTime / BrakeDuration), 0.0f, 1.0f);
	}
	else
	{
		BrakeProgress = FMath::Clamp(BrakeProgress - (DeltaTime / BrakeDuration), 0.0f, 1.0f);
	}

	if (BrakeCurve)
	{
		ActualSpeed *= BrakeCurve->GetFloatValue(BrakeProgress);
	}
	else
	{
		// Simple linear fallback if no curve
		ActualSpeed *= (1.0f - BrakeProgress);
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

void ATrain::OnEngineHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ABridge* Bridge = Cast<ABridge>(OtherActor))
	{
		if (!Bridge->IsFullyRepaired())
		{
			Derail();
		}
	}
}

void ATrain::OnEngineOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABridge* Bridge = Cast<ABridge>(OtherActor))
	{
		if (!Bridge->IsFullyRepaired())
		{
			Derail();
		}
	}
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
