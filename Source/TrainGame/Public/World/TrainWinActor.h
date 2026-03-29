#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrainWinActor.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTrainWinDelegate);

UCLASS()
class TRAINGAME_API ATrainWinActor : public AActor
{
	GENERATED_BODY()

public:
	ATrainWinActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* OverlapVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnTrainWinDelegate OnTrainWin;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
