#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TrainResourceData.generated.h"

/**
 * Data asset for train resources (fuel, etc.)
 */
UCLASS(BlueprintType)
class TRAINGAME_API UTrainResourceData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
	FText ResourceName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
	float FuelValue = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
	UStaticMesh* DisplayMesh;
};
