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
	if (!TrackMesh || !SplineComponent || TrackMeshDistance <= 0.0f)
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

	float TotalLength = SplineComponent->GetSplineLength();
	int32 NumMeshes = FMath::CeilToInt(TotalLength / TrackMeshDistance);

	for (int32 i = 0; i < NumMeshes; ++i)
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

		float StartDist = i * TrackMeshDistance;
		float EndDist = FMath::Min((i + 1) * TrackMeshDistance, TotalLength);

		FVector StartPos = SplineComponent->GetLocationAtDistanceAlongSpline(StartDist, ESplineCoordinateSpace::Local);
		FVector StartTangent = SplineComponent->GetTangentAtDistanceAlongSpline(StartDist, ESplineCoordinateSpace::Local);
		
		FVector EndPos = SplineComponent->GetLocationAtDistanceAlongSpline(EndDist, ESplineCoordinateSpace::Local);
		FVector EndTangent = SplineComponent->GetTangentAtDistanceAlongSpline(EndDist, ESplineCoordinateSpace::Local);

		// Clamp the tangent for consistent spline curve.
		StartTangent = StartTangent.GetClampedToMaxSize(TrackMeshDistance);
		EndTangent = EndTangent.GetClampedToMaxSize(TrackMeshDistance);

		SplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
		
		// Optional: Collision settings
		//SplineMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SplineMesh->SetCollisionProfileName(FName("NoCollision"));
	}
}
