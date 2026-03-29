#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Curves/CurveFloat.h"
#include "Train.generated.h"

class ATrainTrack;
class UTrainEngineComponent;
class UTrainCarData;
class ATrainCar;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTrainDerailed);

UCLASS()
class TRAINGAME_API ATrain : public AActor
{
	GENERATED_BODY()

public:
	ATrain();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Train")
	FOnTrainDerailed OnDerailed;

	UFUNCTION(BlueprintCallable, Category = "Train")
	void Derail();

	UFUNCTION(BlueprintPure, Category = "Train")
	bool IsDerailed() const { return bIsDerailed; }

private:
	bool bIsDerailed = false;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train")
	UCurveFloat* SpeedCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Train")
	float DistanceAlongTrack = 0.0f;

	UFUNCTION(BlueprintCallable, Category = "Train")
	void InitialiseTrain(ATrainTrack* StartingTrack);

protected:
	void UpdatePositionAlongTrack(float DeltaTime);
	void SpawnTrainCars();

	UPROPERTY(BlueprintReadOnly, Category = "Train|Train Cars")
	TArray<ATrainCar*> CarInstances;

	UPROPERTY()
	TArray<ATrainTrack*> TrackHistory;


};
