// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFramework/TrainGameGameMode.h"
#include "GameFramework/TrainGameCharacter.h"
#include "GameFramework/TrainGamePlayerController.h"
#include "GameFramework/TrainHUD.h"

ATrainGameGameMode::ATrainGameGameMode()
{
	// set default pawn class to our C++ character
	DefaultPawnClass = ATrainGameCharacter::StaticClass();

	// set default controller to our C++ controller
	PlayerControllerClass = ATrainGamePlayerController::StaticClass();

	// set default HUD class
	HUDClass = ATrainHUD::StaticClass();
}

void ATrainGameGameMode::TriggerWin()
{
	OnWin.Broadcast();
}

void ATrainGameGameMode::TriggerLoss()
{
	OnLoss.Broadcast();
}
