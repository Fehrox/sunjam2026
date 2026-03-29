#include "World/Bridge.h"
#include "Components/BoxComponent.h"
#include "Train/TrainResource.h"
#include "Train/TrainResourceData.h"

ABridge::ABridge()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComponent->SetGenerateOverlapEvents(true);

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABridge::OnOverlapBegin);
}

void ABridge::BeginPlay()
{
	Super::BeginPlay();
}

void ABridge::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ATrainResource* Resource = Cast<ATrainResource>(OtherActor))
	{
		if (!RequiredResource.IsNull())
		{
			UTrainResourceData* Data = Resource->ResourceData.LoadSynchronous();
			if (Data != RequiredResource.LoadSynchronous())
			{
				return;
			}
		}

		if (!IsFullyRepaired())
		{
			Repair(1);
			Resource->Destroy();
		}
	}
}

void ABridge::Repair(int32 Amount)
{
	if (IsFullyRepaired()) return;

	if (Damage == MaxDamage)
	{
		OnRepairStarted.Broadcast();
	}

	Damage = FMath::Clamp(Damage - Amount, 0, MaxDamage);
	OnRepairing.Broadcast(Damage);

	if (IsFullyRepaired())
	{
		OnFullyRepaired.Broadcast();
	}
}
