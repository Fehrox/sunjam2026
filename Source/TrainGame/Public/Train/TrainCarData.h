#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TrainCarData.generated.h"

class UTrainResourceData;
class UStaticMesh;

UCLASS(BlueprintType)
class TRAINGAME_API UTrainCarData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Train Car")
	FText CarName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Train Car")
	UStaticMesh* Mesh;

	/** The types of resources this car can store */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Train Car")
	TArray<TSoftObjectPtr<UTrainResourceData>> AcceptedResources;

	/** Max capacity of resources this car can hold */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Train Car")
	int32 MaxCapacity = 10;
};
