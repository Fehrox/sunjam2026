// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TrainGameCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInteractionComponent;

/**
 *  A controllable top-down perspective character
 */
UCLASS()
class ATrainGameCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Interaction component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInteractionComponent> InteractionComponent;

public:

	/** Constructor */
	ATrainGameCharacter();

	/** Initialization */
	virtual void BeginPlay() override;

	/** Update */
	virtual void Tick(float DeltaSeconds) override;

	/** Returns the camera component **/
	UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent.Get(); }

	/** Returns the Camera Boom component **/
	USpringArmComponent* GetCameraBoom() const { return CameraBoom.Get(); }

	/** Returns the interaction component **/
	UInteractionComponent* GetInteractionComponent() const { return InteractionComponent.Get(); }

	/** Returns the currently held resource */
	class ATrainResource* GetHeldResource() const { return HeldResource; }

	/** Sets the currently held resource */
	void SetHeldResource(class ATrainResource* Resource) { HeldResource = Resource; }

	/** Called to interact with the environment */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact();

private:
	UPROPERTY()
	class ATrainResource* HeldResource = nullptr;
};

