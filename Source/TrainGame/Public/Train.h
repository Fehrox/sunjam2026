#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Train.generated.h"

class ATrainTrack;
class UTrainEngineComponent;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train")
	ATrainTrack* CurrentTrack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Train")
	UTrainEngineComponent* Engine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train")
	float BaseSpeed = 500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Train")
	float DistanceAlongTrack = 0.0f;

	UFUNCTION(BLueprintCallable, Category = "Train")
	void InitialiseTrain(ATrainTrack* StartingTrack);

private:
	void UpdatePositionAlongTrack(float DeltaTime);
};
