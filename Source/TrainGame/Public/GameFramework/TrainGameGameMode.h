// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TrainGameGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemainingTimeUpdated, float, RemainingTime);

UCLASS()
class ATrainGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	/** Constructor */
	ATrainGameGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game")
	float InitialTimeUntilGameOver = 60.0f;

	FTimerHandle GameTimerHandle;

	UFUNCTION(BlueprintCallable, Category = "Game")
	void AddTime(float Amount);

	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnRemainingTimeUpdated OnRemainingTimeUpdated;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game")
	FOnGameEnded OnWin;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game")
	FOnGameEnded OnLoss;

	UFUNCTION(BlueprintCallable, Category = "Game")
	void TriggerWin();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void TriggerLoss();
};



