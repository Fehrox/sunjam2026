#include "InteractionComponent.h"
#include "InteractionInterface.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f; // Update target less frequently
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateInteractionTarget();
}

void UInteractionComponent::UpdateInteractionTarget()
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	FVector Start = Owner->GetActorLocation();
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	TArray<FOverlapResult> OverlapResults;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(InteractionRange);
	
	bool bHasOverlap = GetWorld()->OverlapMultiByChannel(OverlapResults, Start, FQuat::Identity, ECC_Visibility, Sphere, Params);

	TArray<AActor*> Candidates;
	if (bHasOverlap)
	{
		for (const FOverlapResult& Overlap : OverlapResults)
		{
			AActor* Actor = Overlap.GetActor();
			if (Actor)
			{
				if (Actor->Implements<UInteractable>() || Actor->FindComponentByInterface(UInteractable::StaticClass()))
				{
					Candidates.AddUnique(Actor);
				}
			}
		}
	}

	// Filter by priority
	if (Candidates.Num() > 1)
	{
		int32 MaxPriority = -1000;
		for (AActor* Actor : Candidates)
		{
			int32 Priority = 0;
			if (Actor->Implements<UInteractable>())
			{
				Priority = IInteractable::Execute_GetInteractionPriority(Actor);
			}
			else
			{
				UActorComponent* Comp = Actor->FindComponentByInterface(UInteractable::StaticClass());
				if (Comp)
				{
					Priority = IInteractable::Execute_GetInteractionPriority(Comp);
				}
			}
			if (Priority > MaxPriority)
			{
				MaxPriority = Priority;
			}
		}

		TArray<AActor*> HighPriorityCandidates;
		for (AActor* Actor : Candidates)
		{
			int32 Priority = 0;
			if (Actor->Implements<UInteractable>())
			{
				Priority = IInteractable::Execute_GetInteractionPriority(Actor);
			}
			else
			{
				UActorComponent* Comp = Actor->FindComponentByInterface(UInteractable::StaticClass());
				if (Comp)
				{
					Priority = IInteractable::Execute_GetInteractionPriority(Comp);
				}
			}
			if (Priority == MaxPriority)
			{
				HighPriorityCandidates.Add(Actor);
			}
		}
		Candidates = HighPriorityCandidates;
	}

	AActor* NewTarget = nullptr;

	if (Candidates.Num() == 1)
	{
		NewTarget = Candidates[0];
	}
	else if (Candidates.Num() > 1)
	{
		// More than one interactable, use a linetrace to differentiate
		FHitResult HitResult;
		FVector End = Start + Owner->GetActorForwardVector() * InteractionRange;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor && Candidates.Contains(HitActor))
			{
				NewTarget = HitActor;
			}
		}
		
		// If linetrace didn't hit a specific candidate, pick the closest one
		if (!NewTarget)
		{
			float MinDistSq = FLT_MAX;
			for (AActor* Actor : Candidates)
			{
				float DistSq = FVector::DistSquared(Start, Actor->GetActorLocation());
				if (DistSq < MinDistSq)
				{
					MinDistSq = DistSq;
					NewTarget = Actor;
				}
			}
		}
	}

	if (bShowDebug)
	{
		DrawDebugSphere(GetWorld(), Start, InteractionRange, 12, Candidates.Num() > 0 ? FColor::Green : FColor::Red, false, 0.1f);
		if (Candidates.Num() > 1)
		{
			DrawDebugLine(GetWorld(), Start, Start + Owner->GetActorForwardVector() * InteractionRange, FColor::Yellow, false, 0.1f, 0, 1.0f);
		}
	}

	if (NewTarget != CurrentTarget)
	{
		if (CurrentTarget)
		{
			// Try to call on actor first, then on component if actor doesn't implement it
			if (CurrentTarget->Implements<UInteractable>())
			{
				IInteractable::Execute_OnInteractionFocusExit(CurrentTarget, Owner);
			}
			else
			{
				UActorComponent* Comp = CurrentTarget->FindComponentByInterface(UInteractable::StaticClass());
				if (Comp)
				{
					IInteractable::Execute_OnInteractionFocusExit(Comp, Owner);
				}
			}
		}

		CurrentTarget = NewTarget;

		if (CurrentTarget)
		{
			if (CurrentTarget->Implements<UInteractable>())
			{
				IInteractable::Execute_OnInteractionFocusEnter(CurrentTarget, Owner);
			}
			else
			{
				UActorComponent* Comp = CurrentTarget->FindComponentByInterface(UInteractable::StaticClass());
				if (Comp)
				{
					IInteractable::Execute_OnInteractionFocusEnter(Comp, Owner);
				}
			}
		}
	}
}

void UInteractionComponent::Interact()
{
	if (CurrentTarget)
	{
		if (CurrentTarget->Implements<UInteractable>())
		{
			IInteractable::Execute_Interact(CurrentTarget, GetOwner());
		}
		else
		{
			UActorComponent* Comp = CurrentTarget->FindComponentByInterface(UInteractable::StaticClass());
			if (Comp)
			{
				IInteractable::Execute_Interact(Comp, GetOwner());
			}
		}
	}
}
