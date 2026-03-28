#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "TrainResource.generated.h"

class UTrainResourceData;
class UStaticMeshComponent;

UCLASS()
class TRAINGAME_API ATrainResource : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ATrainResource();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Resource")
	TSoftObjectPtr<UTrainResourceData> ResourceData;

	// IInteractable interface
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetInteractionName_Implementation() const override;
	virtual int32 GetInteractionPriority_Implementation() const override { return 10; }
	// End IInteractable interface

private:
	void OnResourcePickedUp(AActor* Interactor);
	void UpdateMeshFromData();
};
