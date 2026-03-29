// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFramework/TrainGameGameMode.h"
#include "GameFramework/TrainGameCharacter.h"
#include "GameFramework/TrainGamePlayerController.h"
#include "GameFramework/TrainHUD.h"
#include "TimerManager.h"

ATrainGameGameMode::ATrainGameGameMode()
{
	// set default pawn class to our C++ character
	DefaultPawnClass = ATrainGameCharacter::StaticClass();

	// set default controller to our C++ controller
	PlayerControllerClass = ATrainGamePlayerController::StaticClass();

	// set default HUD class
	HUDClass = ATrainHUD::StaticClass();

	PrimaryActorTick.bCanEverTick = true;

	InitialTimeUntilGameOver = 60.0f;
}

void ATrainGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (InitialTimeUntilGameOver > 0.0f)
	{
		GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ATrainGameGameMode::TriggerLoss, InitialTimeUntilGameOver, false);
	}
}

void ATrainGameGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetWorldTimerManager().IsTimerActive(GameTimerHandle))
	{
		float RemainingTime = GetWorldTimerManager().GetTimerRemaining(GameTimerHandle);
		OnRemainingTimeUpdated.Broadcast(RemainingTime);
	}
}

void ATrainGameGameMode::AddTime(float Amount)
{
	float CurrentRemaining = GetWorldTimerManager().GetTimerRemaining(GameTimerHandle);
	if (CurrentRemaining < 0.0f)
	{
		CurrentRemaining = 0.0f;
	}

	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ATrainGameGameMode::TriggerLoss, CurrentRemaining + Amount, false);
}

void ATrainGameGameMode::TriggerWin()
{
	OnWin.Broadcast();
}

void ATrainGameGameMode::TriggerLoss()
{
	OnLoss.Broadcast();
}
