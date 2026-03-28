#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class IInteractable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionDelegate, AActor*, Actor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INTERACTION_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Attempts to interact with the current best target.
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact();

	/**
	 * Returns the current best interaction target.
	 */
	UFUNCTION(BlueprintPure, Category = "Interaction")
	AActor* GetInteractionTarget() const { return CurrentTarget; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractionRange = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractionSphereRadius = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bShowDebug = true;

protected:
	UPROPERTY(BlueprintAssignable)
	FInteractionDelegate OnInteraction;

	UPROPERTY(BlueprintAssignable)
	FInteractionDelegate OnInteractionFocusChange;
private:
	void UpdateInteractionTarget();

	UPROPERTY()
	AActor* CurrentTarget;
};
