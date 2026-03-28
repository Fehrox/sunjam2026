#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "TrainTrack.generated.h"

UCLASS()
class TRAINGAME_API ATrainTrack : public AActor
{
	GENERATED_BODY()

public:
	ATrainTrack();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Track")
	USplineComponent* SplineComponent;

	/** Mesh used for each segment of the track */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	UStaticMesh* TrackMesh;

	/** Material applied to the track mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	UMaterialInterface* TrackMaterial;

	/** Forward axis of the mesh to align with the spline */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	TEnumAsByte<ESplineMeshAxis::Type> ForwardAxis = ESplineMeshAxis::X;

	/** Distance each segment covers along the track */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	float TrackMeshDistance = 1000.0f;

	// Array of possible next tracks (for switches)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Track")
	TArray<ATrainTrack*> NextTracks;

	// Array of previous tracks
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Track")
	TArray<ATrainTrack*> PreviousTracks;

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	void UpdateSplineMeshes();
};
