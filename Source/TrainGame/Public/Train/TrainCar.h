#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrainCar.generated.h"

class UTrainResourceData;
class UTrainCarData;
class UStaticMeshComponent;
class ATrainResource;
class ATrainTrack;

UCLASS()
class TRAINGAME_API ATrainCar : public AActor
{
	GENERATED_BODY()

public:
	ATrainCar();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> RootSceneComponent;

public:
	/** Initialize the train car with its data and track */
	UFUNCTION(BlueprintNativeEvent, Category = "Train Car")
	void Initialise(UTrainCarData* NewCarData, ATrainTrack* StartingTrack, float InitialDistance);
	virtual void Initialise_Implementation(UTrainCarData* NewCarData, ATrainTrack* StartingTrack, float InitialDistance);
	
	/** Update position along the track */
	UFUNCTION(BlueprintCallable, Category = "Train Car")
	void UpdatePosition(ATrainTrack* NewTrack, float NewDistance);

	/** The types of resources this car can store */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Train Car")
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

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Train Car")
	UTrainCarData* CarData;
};
