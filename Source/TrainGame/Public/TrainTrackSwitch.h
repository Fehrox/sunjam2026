#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "TrainTrackSwitch.generated.h"

class ATrainTrack;

UCLASS()
class TRAINGAME_API ATrainTrackSwitch : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ATrainTrackSwitch();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Track Switch")
	ATrainTrack* SourceTrack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Track Switch")
	TArray<ATrainTrack*> TargetTracks;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Track Switch")
	int32 CurrentTargetIndex = 0;

	// IInteractable interface
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetInteractionName_Implementation() const override;
	virtual int32 GetInteractionPriority_Implementation() const override { return 10; }
	// End IInteractable interface

private:
	void ToggleSwitch();
};
