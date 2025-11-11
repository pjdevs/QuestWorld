// Copyright pjdevs. All Rights Reserved.

#include "IPInteractiveActor.h"
#include "IPStateSaveData.h"
#include "IPInteractiveState.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

AIPInteractiveActor::AIPInteractiveActor()
{
	bReplicates = true;

	InteractionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Trigger"));
	InteractionTrigger->SetCollisionProfileName(TEXT("TriggerPawn"));
	RootComponent = InteractionTrigger;

	IndicationTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Indication Trigger"));
	IndicationTrigger->SetCollisionProfileName(TEXT("TriggerPawn"));
	IndicationTrigger->SetupAttachment(RootComponent);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawAtDesiredSize(true);
	WidgetComponent->SetupAttachment(RootComponent);

	InteractiveState = FIPInteractiveState
	{
		.State = EIPInteractiveState::Ready,
		.InteractionCount = 0
	};
	InteractiveName = FText::FromString("Interactive Actor");
	InteractionDescription = FText::FromString("Interact");
	bAutoInteract = false;
}

void AIPInteractiveActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	PossibleInteractors.RemoveAll([](const TWeakObjectPtr<UIPInteractorComponent>& Interactor)
	{
		return !Interactor.IsValid();
	});

	IndicatedInteractors.RemoveAll([](const TWeakObjectPtr<UIPInteractorComponent>& Interactor)
	{
		return !Interactor.IsValid();
	});

	for (const TWeakObjectPtr<UIPInteractorComponent>& Interactor : PossibleInteractors)
	{
		// Interactor->RemoveInteractive(this);
		// Interactor->RemoveInteractiveIndication(this);
		IndicatedInteractors.Remove(Interactor);
	}

	for (const TWeakObjectPtr<UIPInteractorComponent>& Interactor : IndicatedInteractors)
	{
		// Interactor->RemoveInteractiveIndication(this);
	}
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

	PossibleInteractors.Add(Interactor);
	// Interactor->AddInteractive(this);
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

	PossibleInteractors.Remove(Interactor);
	// Interactor->RemoveInteractive(this);

	// End interaction if was interacting with this actor and he left the zone
	if (HasAuthority())
	{
		if (CurrentInteractor == OtherActor)
		{
			EndInteractionInput(OtherActor);	
		}
	}
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
	// Interactor->AddInteractiveIndication(this);
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
	// Interactor->RemoveInteractiveIndication(this);
}

void AIPInteractiveActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &AIPInteractiveActor::HandleInteractionTriggerBeginOverlap);
	InteractionTrigger->OnComponentEndOverlap.AddDynamic(this, &AIPInteractiveActor::HandleInteractionTriggerEndOverlap);

	IndicationTrigger->OnComponentBeginOverlap.AddDynamic(this, &AIPInteractiveActor::HandleIndicationTriggerBeginOverlap);
	IndicationTrigger->OnComponentEndOverlap.AddDynamic(this, &AIPInteractiveActor::HandleIndicationTriggerEndOverlap);
}

void AIPInteractiveActor::StartInteractionInput(AActor* InteractionInstigator)
{
	if (!HasAuthority())
	{
		return;
	}

	const FIPInteractionStatus InteractionStatus = GetInteractionStatus(InteractionInstigator);

	if (!InteractionStatus.bCanStartInteraction)
	{
		// TODO Could make function OnInteractionFailed to track and/or play a "can't do that" feedback
		return;
	}

	OnStartInteractionInput(InteractionInstigator);
}

void AIPInteractiveActor::EndInteractionInput(AActor* InteractionInstigator)
{
	if (!HasAuthority())
	{
		return;
	}
	
	if (InteractionInstigator != CurrentInteractor)
	{
		return;
	}

	OnEndInteractionInput(InteractionInstigator);
}

FIPInteractionStatus AIPInteractiveActor::GetInteractionStatus(AActor* InteractionInstigator) const
{
	FIPInteractionStatus InteractionStatus
	{
		.bCanStartInteraction = InteractiveState.State == EIPInteractiveState::Ready,
	};

	const FIPInteractionStatus AdditionalInteractionStatus = GetInteractionStatusForActor(
		InteractionInstigator,
		InteractiveState
	);

	InteractionStatus.bCanStartInteraction &= AdditionalInteractionStatus.bCanStartInteraction;
	InteractionStatus.ReasonText = AdditionalInteractionStatus.ReasonText;

	return InteractionStatus;
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

FName AIPInteractiveActor::GetUniqueId() const
{
	return UniqueId;
}

bool AIPInteractiveActor::IsSavable() const
{
	return bIsSavable;
}

void AIPInteractiveActor::LoadFromSave(const FIPStateSaveData& SaveData)
{
	if (!HasAuthority())
	{
		return;
	}

	InteractiveState = FIPInteractiveState
	{
		.State = static_cast<EIPInteractiveState>(SaveData.State),
		.InteractionCount = SaveData.Counter
	};

	OnRep_State();
}

FIPStateSaveData AIPInteractiveActor::WriteToSave()
{
	return FIPStateSaveData
	{
		.State = static_cast<uint8>(InteractiveState.State),
		.Counter = InteractiveState.InteractionCount
	};
}

void AIPInteractiveActor::OnRep_State()
{
	NotifyStateChanged();
	DoFeedback(InteractiveState);
}

void AIPInteractiveActor::OnStartInteractionInput_Implementation(AActor* InteractionInstigator)
{
}

void AIPInteractiveActor::OnEndInteractionInput_Implementation(AActor* InteractionInstigator)
{
}

void AIPInteractiveActor::DoFeedback_Implementation(const FIPInteractiveState& NewState)
{
}

void AIPInteractiveActor::StartInteractionPhase(AActor* InteractionInstigator)
{
	if (!HasAuthority())
	{
		return;
	}
	
	CurrentInteractor = InteractionInstigator;
	InteractiveState = FIPInteractiveState
	{
		.State = EIPInteractiveState::Busy,
		.InteractionCount = InteractiveState.InteractionCount,
	};

	OnRep_State();
}

void AIPInteractiveActor::EndInteractionPhase(EIPInteractiveState NextState)
{
	if (!HasAuthority())
	{
		return;
	}
	
	const uint8 NextInteractionCount = InteractiveState.InteractionCount + 1;
	InteractiveState = FIPInteractiveState
	{
		.State = NextState,
		.InteractionCount = NextInteractionCount,
	};

	if (NextState == EIPInteractiveState::Destroyed)
	{
		Destroy();
	}
	else
	{
		OnRep_State();
	}

	CurrentInteractor = nullptr;
}

FIPInteractionStatus AIPInteractiveActor::GetInteractionStatusForActor_Implementation(
	AActor* InteractionInstigator,
	const FIPInteractiveState& CurrentState
) const
{
	return FIPInteractionStatus { .bCanStartInteraction = true };
}

void AIPInteractiveActor::NotifyStateChanged()
{
	IndicatedInteractors.RemoveAll([](const TWeakObjectPtr<UIPInteractorComponent>& Interactor)
	{
		return !Interactor.IsValid();
	});

	// Only notify indicated interactors because possible interactors will be updated on interactor's Tick
	for (const TWeakObjectPtr<UIPInteractorComponent>& Interactor : IndicatedInteractors)
	{
		// Interactor->OnInteractiveStateChanged(this);
	}
}

void AIPInteractiveActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AIPInteractiveActor, InteractiveState);
}
