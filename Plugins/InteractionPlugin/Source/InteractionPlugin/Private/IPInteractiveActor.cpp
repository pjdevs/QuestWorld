// Copyright pjdevs. All Rights Reserved.

#include "IPInteractiveActor.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

AIPInteractiveActor::AIPInteractiveActor()
{
	bReplicates = true;

	InteractionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Trigger"));
	InteractionTrigger->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = InteractionTrigger;

	IndicationTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Indication Trigger"));
	IndicationTrigger->SetCollisionProfileName(TEXT("Trigger"));
	IndicationTrigger->SetupAttachment(RootComponent);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawAtDesiredSize(true);
	WidgetComponent->SetupAttachment(RootComponent);

	State = EIPInteractiveState::Ready;
	bInteractMultipleTimes = true;
	InteractiveName = FText::FromString("Interactive Actor");
	InteractionDescription = FText::FromString("Interact");
	bAutoInteract = false;
}

void AIPInteractiveActor::HandleInteractionTriggerBeginOverlap(
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

	Interactor->AddInteractive(this);
}

void AIPInteractiveActor::HandleInteractionTriggerEndOverlap(
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

	Interactor->RemoveInteractive(this);
}

void AIPInteractiveActor::HandleIndicationTriggerBeginOverlap(
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

	IndicatedInteractors.Add(Interactor);
	Interactor->AddInteractiveIndication(this);
}

void AIPInteractiveActor::HandleIndicationTriggerEndOverlap(
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

	IndicatedInteractors.Remove(Interactor);
	Interactor->RemoveInteractiveIndication(this);
}

void AIPInteractiveActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &AIPInteractiveActor::HandleInteractionTriggerBeginOverlap);
	InteractionTrigger->OnComponentEndOverlap.AddDynamic(this, &AIPInteractiveActor::HandleInteractionTriggerEndOverlap);

	IndicationTrigger->OnComponentBeginOverlap.AddDynamic(this, &AIPInteractiveActor::HandleIndicationTriggerBeginOverlap);
	IndicationTrigger->OnComponentEndOverlap.AddDynamic(this, &AIPInteractiveActor::HandleIndicationTriggerEndOverlap);
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

bool AIPInteractiveActor::CanBeInteracted(AActor* InteractionInstigator) const
{
	return (bInteractMultipleTimes || State == EIPInteractiveState::Ready)
		&& CanBeInteractedBy(InteractionInstigator);
}

FVector AIPInteractiveActor::GetInteractiveLocation() const
{
	const UWidgetComponent* InteractionWidget = GetWidgetComponent();

	return InteractionWidget != nullptr
		? InteractionWidget->GetComponentLocation()
		: GetActorLocation();
}

UWidgetComponent* AIPInteractiveActor::GetWidgetComponent() const
{
	return WidgetComponent;
}

TSubclassOf<UIPInteractionWidget> AIPInteractiveActor::GetInteractionWidgetClass() const
{
	return InteractionWidgetClass;
}

TSubclassOf<UUserWidget> AIPInteractiveActor::GetIndicationWidgetClass() const
{
	return IndicationWidgetClass;
}

TSubclassOf<UUserWidget> AIPInteractiveActor::GetIndicationBlockedWidgetClass() const
{
	return IndicationBlockedWidgetClass;
}

FText AIPInteractiveActor::GetInteractiveName() const
{
	return InteractiveName;
}

FText AIPInteractiveActor::GetInteractionDescription() const
{
	return InteractionDescription;
}

bool AIPInteractiveActor::IsAutoInteractive() const
{
	return bAutoInteract;
}

void AIPInteractiveActor::OnRep_State()
{
	StateChanged();

	if (State == EIPInteractiveState::Interacted || bInteractMultipleTimes)
	{
		DoFeedback();
	}
}

void AIPInteractiveActor::DoInteraction_Implementation(AActor* InteractionInstigator)
{
}

void AIPInteractiveActor::DoFeedback_Implementation()
{
}

bool AIPInteractiveActor::CanBeInteractedBy_Implementation(AActor* InteractionInstigator) const
{
	return true;
}

void AIPInteractiveActor::StateChanged()
{
	IndicatedInteractors.RemoveAll([](const TWeakObjectPtr<UIPInteractorComponent>& Interactor)
	{
		return !Interactor.IsValid();
	});

	for (const TWeakObjectPtr<UIPInteractorComponent>& Interactor : IndicatedInteractors)
	{
		Interactor->OnInteractiveStateChanged(this);
	}
}

void AIPInteractiveActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AIPInteractiveActor, State);
}
