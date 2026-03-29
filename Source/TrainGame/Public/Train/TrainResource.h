#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "TrainResource.generated.h"

class UTrainResourceData;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTGResourceUsedForFuelDelegate, float, NewFuel, float, MaxFuel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTGResourceStoredDelegate, int, NewResourceAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTGResourceVoidDelegate);

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

	/** Called when this resource overlaps with something while thrown */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Throws the resource in a given direction */
	UFUNCTION(BlueprintCallable, Category = "Resource")
	void Throw(FVector Direction);

	// IInteractable interface
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetInteractionName_Implementation() const override;
	virtual int32 GetInteractionPriority_Implementation() const override { return 10; }
	// End IInteractable interface

	/** Updates the mesh based on the resource data */
	UFUNCTION(BlueprintCallable, Category = "Resource")
	void UpdateMeshFromData();

protected:
	UPROPERTY(BlueprintAssignable, Category = "Resource")
	FTGResourceUsedForFuelDelegate OnResourceUsedForFuel;
	UPROPERTY(BlueprintAssignable, Category = "Resource")
	FTGResourceStoredDelegate OnResourceStored;
	UPROPERTY(BlueprintAssignable, Category = "Resource")
	FTGResourceVoidDelegate OnResourcePickedUp;
private:
	void HandleResourcePickedUp(AActor* Interactor);

	bool bIsPickedUp = false;

	UPROPERTY()
	AActor* OwnerCharacter = nullptr;
};
