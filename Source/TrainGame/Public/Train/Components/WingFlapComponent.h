#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "WingFlapComponent.generated.h"

class USceneComponent;

struct FWingFlapResolvedComponentState
{
	TWeakObjectPtr<USceneComponent> Component;
	TWeakObjectPtr<USceneComponent> ParentComponent;
	FQuat InitialRelativeRotation = FQuat::Identity;
	FQuat InitialWorldRotation = FQuat::Identity;

	bool IsValid() const
	{
		return Component.IsValid();
	}
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TRAINGAME_API UWingFlapComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWingFlapComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wing Flap")
	bool bAnimateWings = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wing Flap", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float WingFlapArcDegrees = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wing Flap", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float WingFlapFrequency = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wing Flap")
	float LeftWingAngleMultiplier = -1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wing Flap")
	float RightWingAngleMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wing Flap", meta = (UseComponentPicker, AllowedClasses = "/Script/Engine.SceneComponent"))
	FComponentReference LeftWing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wing Flap", meta = (UseComponentPicker, AllowedClasses = "/Script/Engine.SceneComponent"))
	FComponentReference RightWing;

	UFUNCTION(BlueprintCallable, Category = "Wing Flap")
	void RefreshWingReferences();

private:
	void CacheWingReference(const FComponentReference& WingReference, FWingFlapResolvedComponentState& OutWingState, const TCHAR* WingLabel) const;
	static void UpdateWingComponent(const FWingFlapResolvedComponentState& WingState, float FlapAngleDegrees);

	FWingFlapResolvedComponentState LeftWingState;
	FWingFlapResolvedComponentState RightWingState;
	float AnimationTime = 0.0f;
};
