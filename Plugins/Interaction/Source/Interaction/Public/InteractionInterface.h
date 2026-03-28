#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class INTERACTION_API IInteractable
{
	GENERATED_BODY()

public:
	/**
	 * Called on the interactable actor when interacted with.
	 * @param Interactor The actor initiating the interaction.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(AActor* Interactor);

	/**
	 * Called when the interactor starts focusing on this object (e.g., hover).
	 * @param Interactor The actor initiating the focus.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteractionFocusEnter(AActor* Interactor);

	/**
	 * Called when the interactor stops focusing on this object.
	 * @param Interactor The actor that was focusing on this.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteractionFocusExit(AActor* Interactor);

	/**
	 * Returns the display name of this interactable for UI purposes.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FText GetInteractionName() const;

	/**
	 * Returns the priority of this interactable. Higher priority is prioritized over lower priority.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	int32 GetInteractionPriority() const;
};
