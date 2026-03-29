#include "TrainResource.h"
#include "TrainResourceData.h"
#include "TrainCar.h"
#include "TrainEngine.h"
#include "TrainGameCharacter.h"
#include "Components/StaticMeshComponent.h"

ATrainResource::ATrainResource()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetNotifyRigidBodyCollision(true);
	MeshComponent->OnComponentHit.AddDynamic(this, &ATrainResource::OnHit);
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ATrainResource::OnOverlapBegin);
}

void ATrainResource::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UpdateMeshFromData();
}

void ATrainResource::BeginPlay()
{
	Super::BeginPlay();
	UpdateMeshFromData();
}

void ATrainResource::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsPickedUp && OwnerCharacter)
	{
		FVector TargetLocation = OwnerCharacter->GetActorLocation() + FVector(0.0f, 0.0f, FloatHeight);
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaSeconds, 10.0f));
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), FRotator::ZeroRotator, DeltaSeconds, 5.0f));
	}
}

void ATrainResource::UpdateMeshFromData()
{
	if (ResourceData.ToSoftObjectPath().IsValid())
	{
		UTrainResourceData* ResourceDataHardPtr = ResourceData.LoadSynchronous();
		if (ResourceDataHardPtr && ResourceDataHardPtr->DisplayMesh)
		{
			MeshComponent->SetStaticMesh(ResourceDataHardPtr->DisplayMesh);
		}
	}
}

void ATrainResource::Interact_Implementation(AActor* Interactor)
{
	if (!bIsPickedUp)
	{
		HandleResourcePickedUp(Interactor);
	}
}

FText ATrainResource::GetInteractionName_Implementation() const
{
	if (ResourceData)
	{
		return FText::Format(NSLOCTEXT("TrainGame", "InteractResource", "Pick up {0}"), ResourceData->ResourceName);
	}
	return NSLOCTEXT("TrainGame", "InteractResourceDefault", "Pick up resource");
}

void ATrainResource::HandleResourcePickedUp(AActor* Interactor)
{
	if (ATrainGameCharacter* Character = Cast<ATrainGameCharacter>(Interactor))
	{
		if (Character->GetHeldResource() == nullptr)
		{
			bIsPickedUp = true;
			OwnerCharacter = Character;
			Character->SetHeldResource(this);

			OnResourcePickedUp.Broadcast();

			MeshComponent->SetSimulatePhysics(false);
			MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void ATrainResource::Throw(FVector Direction)
{
	bIsPickedUp = false;
	OwnerCharacter = nullptr;

	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->AddImpulse(Direction * ThrowForce, NAME_None, true);
}

void ATrainResource::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bIsPickedUp) return;

	if (ATrainCar* TrainCar = Cast<ATrainCar>(OtherActor))
	{
		if (TrainCar->TryStoreResource(this))
		{
			OnResourceStored.Broadcast(TrainCar->StoredResources.Num());
			return;
		}
	}

	if (OtherComp)
	{
		if (UTrainEngineComponent* EngineComp = Cast<UTrainEngineComponent>(OtherComp))
		{
			UTrainResourceData* Data = ResourceData.LoadSynchronous();
			if (Data && Data->FuelValue > 0)
			{
				EngineComp->AddFuel(Data->FuelValue);
				OnResourceUsedForFuel.Broadcast(EngineComp->CurrentFuel, EngineComp->MaxFuel);
				Destroy();
				return;
			}
		}
	}
}

void ATrainResource::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsPickedUp) return;

	if (ATrainCar* TrainCar = Cast<ATrainCar>(OtherActor))
	{
		if (TrainCar->TryStoreResource(this))
		{
			return;
		}
	}

	if (OtherComp)
	{
		if (UTrainEngineComponent* EngineComp = Cast<UTrainEngineComponent>(OtherComp))
		{
			UTrainResourceData* Data = ResourceData.LoadSynchronous();
			if (Data && Data->FuelValue > 0)
			{
				EngineComp->AddFuel(Data->FuelValue);
				Destroy();
				return;
			}
		}
	}
}
