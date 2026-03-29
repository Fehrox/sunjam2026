#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TrainHUDWidget.generated.h"

class ATrain;

/**
 * 
 */
UCLASS()
class TRAINGAME_API UTrainHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void UpdateFuel(float CurrentFuel, float MaxFuel);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void InitialiseHUD(ATrain* TrainActor);

protected:
	UFUNCTION()
	void OnFuelChanged(float CurrentFuel, float MaxFuel);
};
