#include "TrainHUDWidget.h"
#include "Train.h"
#include "TrainEngine.h"

void UTrainHUDWidget::InitialiseHUD(ATrain* TrainActor)
{
	if (TrainActor && TrainActor->Engine)
	{
		TrainActor->Engine->OnFuelChange.AddDynamic(this, &UTrainHUDWidget::OnFuelChanged);
		TrainActor->Engine->OnFuelAdded.AddDynamic(this, &UTrainHUDWidget::OnFuelChanged);
		
		// Initial update
		UpdateFuel(TrainActor->Engine->CurrentFuel, TrainActor->Engine->MaxFuel);
	}
}

void UTrainHUDWidget::OnFuelChanged(float CurrentFuel, float MaxFuel)
{
	UpdateFuel(CurrentFuel, MaxFuel);
}
