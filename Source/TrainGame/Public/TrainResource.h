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
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Resource")
	TSoftObjectPtr<UTrainResourceData> ResourceData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	float FloatHeight = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	float ThrowForce = 1500.00f;

	/** Called when this resource hits something while thrown */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Throws the resource in a given direction */
	UFUNCTION(BlueprintCallable, Category = "Resource")
	void Throw(FVector Direction);

	// IInteractable interface
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetInteractionName_Implementation() const override;
	virtual int32 GetInteractionPriority_Implementation() const override { return 10; }
	// End IInteractable interface

private:
	void OnResourcePickedUp(AActor* Interactor);
	void UpdateMeshFromData();

	bool bIsPickedUp = false;

	UPROPERTY()
	AActor* OwnerCharacter = nullptr;
};
