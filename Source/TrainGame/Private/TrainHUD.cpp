#include "TrainHUD.h"
#include "TrainHUDWidget.h"
#include "Blueprint/UserWidget.h"
#include "Train.h"
#include "TrainEngine.h"

ATrainHUD::ATrainHUD()
{
}

void ATrainHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UTrainHUDWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}
}

void ATrainHUD::InitialiseHUD(ATrain* TrainActor)
{
	if (HUDWidget && TrainActor)
	{
		HUDWidget->InitialiseHUD(TrainActor);
	}
}
