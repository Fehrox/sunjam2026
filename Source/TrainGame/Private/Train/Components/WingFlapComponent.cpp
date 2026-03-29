#include "Train/Components/WingFlapComponent.h"

#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY_STATIC(LogWingFlapComponent, Log, All);

namespace
{
	bool HasExplicitComponentReference(const FComponentReference& ComponentReference)
	{
		return ComponentReference.ComponentProperty != NAME_None
			|| !ComponentReference.PathToComponent.IsEmpty()
			|| ComponentReference.OverrideComponent.IsValid()
			|| ComponentReference.OtherActor.IsValid();
	}

	FName GetFallbackWingComponentName(const TCHAR* WingLabel)
	{
		return FCString::Strcmp(WingLabel, TEXT("LeftWing")) == 0 ? TEXT("SM_Wing_L") : TEXT("SM_Wing_R");
	}

	USceneComponent* FindWingComponentByName(AActor& Owner, const FName ComponentName)
	{
		const FString RequestedName = ComponentName.ToString();
		TArray<USceneComponent*> SceneComponents;
		Owner.GetComponents<USceneComponent>(SceneComponents);

		for (USceneComponent* SceneComponent : SceneComponents)
		{
			if (!SceneComponent || SceneComponent == Owner.GetRootComponent())
			{
				continue;
			}

			if (SceneComponent->GetFName() == ComponentName || SceneComponent->GetName().StartsWith(RequestedName))
			{
				return SceneComponent;
			}
		}

		return nullptr;
	}
}

UWingFlapComponent::UWingFlapComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWingFlapComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
	{
		PrimaryComponentTick.AddPrerequisite(Owner, Owner->PrimaryActorTick);
	}

	RefreshWingReferences();
}

void UWingFlapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!LeftWingState.IsValid() || !RightWingState.IsValid())
	{
		return;
	}

	if (bAnimateWings)
	{
		AnimationTime += DeltaTime;
	}

	const float FlapAngleDegrees = bAnimateWings
		? 0.5f * WingFlapArcDegrees * FMath::Sin(AnimationTime * WingFlapFrequency * UE_TWO_PI)
		: 0.0f;

	UpdateWingComponent(LeftWingState, FlapAngleDegrees * LeftWingAngleMultiplier);
	UpdateWingComponent(RightWingState, FlapAngleDegrees * RightWingAngleMultiplier);
}

void UWingFlapComponent::RefreshWingReferences()
{
	CacheWingReference(LeftWing, LeftWingState, TEXT("LeftWing"));
	CacheWingReference(RightWing, RightWingState, TEXT("RightWing"));
}

void UWingFlapComponent::CacheWingReference(const FComponentReference& WingReference, FWingFlapResolvedComponentState& OutWingState, const TCHAR* WingLabel) const
{
	OutWingState = FWingFlapResolvedComponentState{};

	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	USceneComponent* WingComponent = nullptr;
	if (HasExplicitComponentReference(WingReference))
	{
		WingComponent = Cast<USceneComponent>(WingReference.GetComponent(Owner));
	}

	if (!WingComponent || WingComponent == Owner->GetRootComponent())
	{
		WingComponent = FindWingComponentByName(*Owner, GetFallbackWingComponentName(WingLabel));
	}

	if (!WingComponent)
	{
		UE_LOG(LogWingFlapComponent, Warning, TEXT("%s on %s could not resolve %s. Set the reference explicitly or ensure the fallback component name exists."), *GetName(), *GetNameSafe(Owner), WingLabel);
		return;
	}

	OutWingState.Component = WingComponent;
	OutWingState.ParentComponent = WingComponent->GetAttachParent();
	OutWingState.InitialRelativeRotation = WingComponent->GetRelativeRotation().Quaternion();
	OutWingState.InitialWorldRotation = WingComponent->GetComponentQuat();
}

void UWingFlapComponent::UpdateWingComponent(const FWingFlapResolvedComponentState& WingState, float FlapAngleDegrees)
{
	USceneComponent* WingComponent = WingState.Component.Get();
	if (!WingComponent)
	{
		return;
	}

	const USceneComponent* ParentComponent = WingState.ParentComponent.Get();
	const FQuat BaseWorldRotation = ParentComponent
		? ParentComponent->GetComponentQuat() * WingState.InitialRelativeRotation
		: WingState.InitialWorldRotation;

	const FQuat FlapOffset = FQuat(FVector::ForwardVector, FMath::DegreesToRadians(FlapAngleDegrees));
	WingComponent->SetWorldRotation((FlapOffset * BaseWorldRotation).Rotator());
}
