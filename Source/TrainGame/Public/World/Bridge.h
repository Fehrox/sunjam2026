#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bridge.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBridgeRepairing, int32, CurrentDamage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBridgeRepairStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBridgeFullyRepaired);

UCLASS()
class TRAINGAME_API ABridge : public AActor
{
	GENERATED_BODY()

public:
	ABridge();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	int32 Damage = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	int32 MaxDamage = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	TSoftObjectPtr<class UTrainResourceData> RequiredResource;

public:
	UPROPERTY(BlueprintAssignable, Category = "Bridge")
	FOnBridgeRepairing OnRepairing;

	UPROPERTY(BlueprintAssignable, Category = "Bridge")
	FOnBridgeRepairStarted OnRepairStarted;

	UPROPERTY(BlueprintAssignable, Category = "Bridge")
	FOnBridgeFullyRepaired OnFullyRepaired;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Bridge")
	void Repair(int32 Amount = 1);

	UFUNCTION(BlueprintPure, Category = "Bridge")
	bool IsFullyRepaired() const { return Damage <= 0; }

	UFUNCTION(BlueprintPure, Category = "Bridge")
	int32 GetCurrentDamage() const { return Damage; }
};
