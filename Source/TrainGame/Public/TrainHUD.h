#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TrainHUD.generated.h"

class UTrainHUDWidget;
class ATrain;

/**
 * 
 */
UCLASS()
class TRAINGAME_API ATrainHUD : public AHUD
{
	GENERATED_BODY()

public:
	ATrainHUD();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UTrainHUDWidget> HUDWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	TObjectPtr<UTrainHUDWidget> HUDWidget;

protected:
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void InitialiseHUD(ATrain* TrainActor);
};
