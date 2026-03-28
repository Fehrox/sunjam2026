#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Train.generated.h"

class ATrainTrack;
class UTrainEngineComponent;
class UTrainCarData;
class ATrainCar;

UCLASS()
class TRAINGAME_API ATrain : public AActor
{
	GENERATED_BODY()

public:
	ATrain();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train|Train Cars")
	TSubclassOf<ATrainCar> TrainCarClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train|Train Cars")
	TArray<UTrainCarData*> TrainCarsToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train|Train Cars")
	float CarDistance = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train|Track")
	ATrainTrack* CurrentTrack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train|Track")
	int32 MaxTrackHistory = 10;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Train")
	UTrainEngineComponent* Engine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train")
	float BaseSpeed = 500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Train")
	float DistanceAlongTrack = 0.0f;

	UFUNCTION(BlueprintCallable, Category = "Train")
	void InitialiseTrain(ATrainTrack* StartingTrack);

private:
	void UpdatePositionAlongTrack(float DeltaTime);
	void SpawnTrainCars();

	UPROPERTY()
	TArray<ATrainCar*> CarInstances;

	UPROPERTY()
	TArray<ATrainTrack*> TrackHistory;


};
