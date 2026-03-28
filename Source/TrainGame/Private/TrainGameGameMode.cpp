// Copyright Epic Games, Inc. All Rights Reserved.

#include "TrainGameGameMode.h"
#include "TrainGameCharacter.h"
#include "TrainGamePlayerController.h"
#include "TrainHUD.h"

ATrainGameGameMode::ATrainGameGameMode()
{
	// set default pawn class to our C++ character
	DefaultPawnClass = ATrainGameCharacter::StaticClass();

	// set default controller to our C++ controller
	PlayerControllerClass = ATrainGamePlayerController::StaticClass();

	// set default HUD class
	HUDClass = ATrainHUD::StaticClass();
}
