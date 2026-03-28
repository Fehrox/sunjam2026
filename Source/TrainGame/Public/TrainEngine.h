#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "TrainEngine.generated.h"

UCLASS()
class TRAINGAME_API ATrainEngine : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ATrainEngine();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine")
	float MaxFuel = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Engine")
	float CurrentFuel = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine")
	float ConsumptionRate = 2.0f;

	// IInteractable interface
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetInteractionName_Implementation() const override;
	virtual int32 GetInteractionPriority_Implementation() const override { return 5; }
	// End IInteractable interface

	UFUNCTION(BlueprintPure, Category = "Engine")
	float GetFuelRatio() const { return CurrentFuel / MaxFuel; }

	UFUNCTION(BlueprintCallable, Category = "Engine")
	void AddFuel(float Amount);
};
