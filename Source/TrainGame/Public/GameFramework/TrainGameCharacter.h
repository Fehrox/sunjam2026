// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TrainGameCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInteractionComponent;
class UDamageType;

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

	virtual void FellOutOfWorld(const UDamageType& DmgType) override;

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

	/** Zoom camera based on character being indoors */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void SetIndoors(bool bInIndoors);

private:
	bool TriggerFallLoss();

	UPROPERTY()
	class ATrainResource* HeldResource = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game", meta = (AllowPrivateAccess = "true"))
	float FallLossZThreshold = -3.f;

	bool bHasTriggeredFallLoss = false;

	/** Default camera boom target arm length */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera", meta = (AllowPrivateAccess = "true"))
	float DefaultZoomDistance = 800.f;

	/** Target arm length when character is indoors */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera", meta = (AllowPrivateAccess = "true"))
	float IndoorZoomDistance = 400.f;

	/** Zoom interpolation speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera", meta = (AllowPrivateAccess = "true"))
	float ZoomInterpSpeed = 5.f;

	/** Whether character is currently indoors */
	bool bIsIndoors = false;
};

