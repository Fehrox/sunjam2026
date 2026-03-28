#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Train.generated.h"

class ATrainTrack;
class ATrainEngine;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train")
	ATrainEngine* Engine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train")
	float BaseSpeed = 500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Train")
	float DistanceAlongTrack = 0.0f;

private:
	void UpdatePositionAlongTrack(float DeltaTime);
};
