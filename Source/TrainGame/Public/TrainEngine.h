#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "InteractionInterface.h"
#include "TrainEngine.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTGFuelChangeDelegate, float, CurrentFuel, float, MaxFuel);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TRAINGAME_API UTrainEngineComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UTrainEngineComponent();
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Engine")
	FTGFuelChangeDelegate OnFuelChange;

	UPROPERTY(BlueprintAssignable, Category = "Engine")
	FTGFuelChangeDelegate OnFuelAdded;

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine")
	float MaxFuel = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Engine")
	float CurrentFuel = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine")
	float ConsumptionRate = 2.0f;

	UFUNCTION(BlueprintPure, Category = "Engine")
	float GetFuelRatio() const { return CurrentFuel / MaxFuel; }

	UFUNCTION(BlueprintCallable, Category = "Engine")
	void AddFuel(float Amount);

protected:

	/** Called when this resource hits something while thrown */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Called when this resource overlaps with something while thrown */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
