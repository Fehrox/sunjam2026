#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Curves/CurveFloat.h"
#include "Components/BoxComponent.h"
#include "InteractionInterface.h"
#include "Train.generated.h"

class ATrain;
class ATrainTrack;
class UTrainEngineComponent;
class UTrainCarData;
class ATrainCar;
class ABridge;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBrakeSwitchToggled, bool, bIsApplied);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TRAINGAME_API UTrainBrakeSwitch : public UBoxComponent, public IInteractable
{
	GENERATED_BODY()

public:
	UTrainBrakeSwitch();

	UPROPERTY(BlueprintAssignable, Category = "Brake Switch")
	FOnBrakeSwitchToggled OnBrakeSwitchToggled;

	// IInteractable interface
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetInteractionName_Implementation() const override;
	virtual int32 GetInteractionPriority_Implementation() const override { return 10; }
	// End IInteractable interface
};

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

	UFUNCTION(BlueprintCallable, Category = "Train")
	void ToggleBrakes();

	UFUNCTION(BlueprintPure, Category = "Train")
	bool AreBrakesEnabled() const { return bBrakesEnabled; }

private:
	bool bIsDerailed = false;
	bool bBrakesEnabled = false;
	float BrakeProgress = 0.0f;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Train")
	UTrainBrakeSwitch* BrakeSwitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train")
	float BaseSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train")
	UCurveFloat* SpeedCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train|Brakes")
	UCurveFloat* BrakeCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train|Brakes")
	float BrakeDuration = 2.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Train")
	float DistanceAlongTrack = 0.0f;

	UFUNCTION(BlueprintCallable, Category = "Train")
	void InitialiseTrain(ATrainTrack* StartingTrack);

protected:
	void UpdatePositionAlongTrack(float DeltaTime);
	void SpawnTrainCars();

	UFUNCTION()
	void OnEngineHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnEngineOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadOnly, Category = "Train|Train Cars")
	TArray<ATrainCar*> CarInstances;

	UPROPERTY()
	TArray<ATrainTrack*> TrackHistory;


};
