#include "GameFramework/TrainHUD.h"
#include "GameFramework/TrainHUDWidget.h"
#include "Blueprint/UserWidget.h"
#include "Train/Train.h"
#include "Train/TrainEngine.h"

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
