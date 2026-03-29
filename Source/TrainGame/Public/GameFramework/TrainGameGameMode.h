// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TrainGameGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameEnded);

UCLASS()
class ATrainGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	/** Constructor */
	ATrainGameGameMode();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game")
	FOnGameEnded OnWin;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game")
	FOnGameEnded OnLoss;

	UFUNCTION(BlueprintCallable, Category = "Game")
	void TriggerWin();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void TriggerLoss();
};



