#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrainCar.generated.h"

class UTrainResourceData;
class UStaticMeshComponent;
class ATrainResource;

UCLASS()
class TRAINGAME_API ATrainCar : public AActor
{
	GENERATED_BODY()

public:
	ATrainCar();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

public:
	/** The types of resources this car can store */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train Car")
	TArray<TSoftObjectPtr<UTrainResourceData>> AcceptedResources;

	/** The resources currently stored in this car */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Train Car")
	TArray<TSoftObjectPtr<UTrainResourceData>> StoredResources;

	/** Max capacity of resources this car can hold */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train Car")
	int32 MaxCapacity = 10;

	/** Attempts to store a resource in this car */
	UFUNCTION(BlueprintCallable, Category = "Train Car")
	bool TryStoreResource(ATrainResource* Resource);

	/** Checks if a resource can be stored in this car */
	UFUNCTION(BlueprintPure, Category = "Train Car")
	bool CanStoreResource(UTrainResourceData* ResourceData) const;
};
