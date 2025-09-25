// Copyright pjdevs. All Rights Reserved.

#include "IPInteractiveActor.h"
#include "Net/UnrealNetwork.h"

AIPInteractiveActor::AIPInteractiveActor()
{
	bReplicates = true;

	InteractionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Trigger"));
	InteractionTrigger->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = InteractionTrigger;

	State = EIPInteractiveState::Ready;
	bInteractMultipleTimes = true;
	InteractionDescription = "Interact";
	bAutoInteract = false;
}

void AIPInteractiveActor::HandleTriggerBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult
)
{
	if (!OtherActor)
	{
		return;
	}

	// Try to get the Interactor component from the other actor
	auto* Interactor = Cast<UIPInteractorComponent>(
		OtherActor->GetComponentByClass(UIPInteractorComponent::StaticClass())
	);

	if (!Interactor)
	{
		return;
	}

	PossibleInteractors.Add(Interactor);
	Interactor->AddInteractive(this);

	if (bAutoInteract)
	{
		Interactor->Interact();
	}
}

void AIPInteractiveActor::HandleTriggerEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex
)
{
	if (!OtherActor)
	{
		return;
	}

	// Try to get the Interactor component from the other actor
	auto* Interactor = Cast<UIPInteractorComponent>(
		OtherActor->GetComponentByClass(UIPInteractorComponent::StaticClass())
	);

	if (!Interactor)
	{
		return;
	}
	
	PossibleInteractors.Remove(Interactor);
	Interactor->RemoveInteractive(this);
}

void AIPInteractiveActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &AIPInteractiveActor::HandleTriggerBeginOverlap);
	InteractionTrigger->OnComponentEndOverlap.AddDynamic(this, &AIPInteractiveActor::HandleTriggerEndOverlap);
}

void AIPInteractiveActor::Interact(AActor* InteractionInstigator)
{
	if (!HasAuthority())
	{
		return;
	}
	
	if (!CanBeInteracted(InteractionInstigator))
	{
		return;
	}

	DoInteraction(InteractionInstigator);

	if (bInteractMultipleTimes)
	{
		State = State == EIPInteractiveState::Interacted ? EIPInteractiveState::Ready : EIPInteractiveState::Interacted;
	}
	else
	{
		State = EIPInteractiveState::Interacted;
	}

	OnRep_State();
}

bool AIPInteractiveActor::CanBeInteracted(AActor* InteractionInstigator)
{
	return bInteractMultipleTimes || State == EIPInteractiveState::Ready;
}

FString AIPInteractiveActor::GetInteractionDescription() const
{
	return InteractionDescription;
}

bool AIPInteractiveActor::IsAutoInteractive() const
{
	return bAutoInteract;
}

void AIPInteractiveActor::OnRep_State()
{
	if (State != EIPInteractiveState::Interacted && !bInteractMultipleTimes)
	{
		return;
	}

	for (auto* Interactor : PossibleInteractors)
	{
		if (!bInteractMultipleTimes || !CanBeInteracted(Interactor->GetOwner()))
		{
			Interactor->RemoveInteractive(this);
		}
	}

	DoFeedback();
}

void AIPInteractiveActor::DoInteraction_Implementation(AActor* InteractionInstigator)
{
}

void AIPInteractiveActor::DoFeedback_Implementation()
{
}

void AIPInteractiveActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AIPInteractiveActor, State);
}
