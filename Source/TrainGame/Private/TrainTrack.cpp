#include "TrainTrack.h"
#include "Components/SplineMeshComponent.h"

ATrainTrack::ATrainTrack()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	RootComponent = SplineComponent;

	ForwardAxis = ESplineMeshAxis::X;
}

void ATrainTrack::BeginPlay()
{
	Super::BeginPlay();
}

void ATrainTrack::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdateSplineMeshes();
}

void ATrainTrack::UpdateSplineMeshes()
{
	if (!TrackMesh || !SplineComponent)
	{
		return;
	}

	// We don't want to destroy components that were created as default subobjects, 
	// but since we are creating them dynamically in OnConstruction, we should clear old ones.
	// This approach is common in simple spline-based visual generators.
	TArray<USceneComponent*> SplineMeshChildren;
	SplineComponent->GetChildrenComponents(true, SplineMeshChildren);
	for (USceneComponent* Child : SplineMeshChildren)
	{
		if (USplineMeshComponent* SplineMesh = Cast<USplineMeshComponent>(Child))
		{
			SplineMesh->DestroyComponent();
		}
	}

	int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();

	for (int32 i = 0; i < NumPoints - 1; ++i)
	{
		USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this);
		SplineMesh->SetMobility(EComponentMobility::Movable); // or Static if the track is static
		SplineMesh->SetStaticMesh(TrackMesh);
		if (TrackMaterial)
		{
			SplineMesh->SetMaterial(0, TrackMaterial);
		}
		
		SplineMesh->SetForwardAxis(ForwardAxis);
		
		SplineMesh->RegisterComponent();
		SplineMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

		FVector StartPos, StartTangent, EndPos, EndTangent;
		SplineComponent->GetLocationAndTangentAtSplinePoint(i, StartPos, StartTangent, ESplineCoordinateSpace::Local);
		SplineComponent->GetLocationAndTangentAtSplinePoint(i + 1, EndPos, EndTangent, ESplineCoordinateSpace::Local);

		SplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
		
		// Optional: Collision settings
		//SplineMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SplineMesh->SetCollisionProfileName(FName("NoCollision"));
	}
}
