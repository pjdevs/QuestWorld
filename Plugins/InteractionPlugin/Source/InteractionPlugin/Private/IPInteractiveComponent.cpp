// Copyright pjdevs. All Rights Reserved.


#include "IPInteractiveComponent.h"
#include "IPInteractionHandler.h"
#include "IPInteractorComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"


#define INTERACTION_HANDLER(FunctionName, ...) \
if (GetOwner() && GetOwner()->Implements<UIPInteractionHandler>()) \
{ \
if (IIPInteractionHandler* Handler = Cast<IIPInteractionHandler>(GetOwner())) \
{ \
Handler->FunctionName(__VA_ARGS__); \
} \
else \
{ \
IIPInteractionHandler::Execute_##FunctionName(GetOwner(), __VA_ARGS__); \
} \
}

#define INTERACTION_HANDLER_RETURN(FunctionName, DefaultValue, ...) \
[&] \
{ \
	if (GetOwner() && GetOwner()->Implements<UIPInteractionHandler>()) \
	{ \
		if (IIPInteractionHandler* Handler = Cast<IIPInteractionHandler>(GetOwner())) \
		{ \
			return Handler->##FunctionName(__VA_ARGS__); \
		} \
		else \
		{ \
			return IIPInteractionHandler::Execute_##FunctionName(GetOwner(), __VA_ARGS__); \
		} \
	} \
	return DefaultValue; \
}()

UIPInteractiveComponent::UIPInteractiveComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	InteractiveState = FIPInteractiveState
	{
		.State = EIPInteractiveState::Ready,
		.InteractionCount = 0
	};
	InteractiveName = FText::FromString("Interactive Actor");
	InteractionDescription = FText::FromString("Interact");
	bIsAutoInteractive = false;
}

void UIPInteractiveComponent::BeginPlay()
{
	Super::BeginPlay();

	if (
		TArray<UActorComponent*> InteractionTriggers = GetOwner()->GetComponentsByTag(
			UPrimitiveComponent::StaticClass(),
			FName("InteractionTrigger")
		);
		InteractionTriggers.Num() > 0
	)
	{
		InteractionTrigger = Cast<UPrimitiveComponent>(InteractionTriggers[0]); 
	}

	if (
		TArray<UActorComponent*> IndicationTriggers = GetOwner()->GetComponentsByTag(
			UPrimitiveComponent::StaticClass(),
			FName("IndicationTrigger")
		);
		IndicationTriggers.Num() > 0
	)
	{
		IndicationTrigger = Cast<UPrimitiveComponent>(IndicationTriggers[0]); 
	}

	WidgetComponent = GetOwner()->GetComponentByClass<UWidgetComponent>();
	
	if (InteractionTrigger)
	{
		InteractionTrigger->OnComponentBeginOverlap.AddDynamic(
			this,
			&UIPInteractiveComponent::HandleInteractionTriggerBeginOverlap
		);
		InteractionTrigger->OnComponentEndOverlap.AddDynamic(
			this,
			&UIPInteractiveComponent::HandleInteractionTriggerEndOverlap
		);
	}

	if (IndicationTrigger)
	{
		IndicationTrigger->OnComponentBeginOverlap.AddDynamic(
			this,
			&UIPInteractiveComponent::HandleIndicationTriggerBeginOverlap
		);
		IndicationTrigger->OnComponentEndOverlap.AddDynamic(
			this,
			&UIPInteractiveComponent::HandleIndicationTriggerEndOverlap
		);
	}
}

void UIPInteractiveComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
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
		Interactor->RemoveInteractive(this);
		Interactor->RemoveInteractiveIndication(this);
		IndicatedInteractors.Remove(Interactor);
	}

	for (const TWeakObjectPtr<UIPInteractorComponent>& Interactor : IndicatedInteractors)
	{
		Interactor->RemoveInteractiveIndication(this);
	}
}

void UIPInteractiveComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UIPInteractiveComponent, InteractiveState);
}

void UIPInteractiveComponent::StartInteractionInput(AActor* InteractionInstigator)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	const FIPInteractionStatus InteractionStatus = GetInteractionStatus(InteractionInstigator);

	if (!InteractionStatus.bCanStartInteraction)
	{
		// TODO Could make function OnInteractionFailed to track and/or play a "can't do that" feedback
		return;
	}

	INTERACTION_HANDLER(OnStartInteractionInput, InteractionInstigator)
}

void UIPInteractiveComponent::EndInteractionInput(AActor* InteractionInstigator)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}
	
	if (InteractionInstigator != CurrentInteractor)
	{
		return;
	}

	INTERACTION_HANDLER(OnEndInteractionInput, InteractionInstigator)
}

FIPInteractionStatus UIPInteractiveComponent::GetInteractionStatus(AActor* InteractionInstigator) const
{
	FIPInteractionStatus InteractionStatus
	{
		.bCanStartInteraction = InteractiveState.State == EIPInteractiveState::Ready,
	};

	const FIPInteractionStatus& AdditionalInteractionStatus = INTERACTION_HANDLER_RETURN(
		GetInteractionStatusForActor,
		FIPInteractionStatus { .bCanStartInteraction = true },
		InteractionInstigator,
		InteractiveState
	);

	InteractionStatus.bCanStartInteraction &= AdditionalInteractionStatus.bCanStartInteraction;
	InteractionStatus.ReasonText = AdditionalInteractionStatus.ReasonText;

	return InteractionStatus;
}

FVector UIPInteractiveComponent::GetInteractiveLocation() const
{
	const UWidgetComponent* InteractionWidget = GetWidgetComponent();

	return InteractionWidget != nullptr
		? InteractionWidget->GetComponentLocation()
		: GetOwner()->GetActorLocation();
}

UWidgetComponent* UIPInteractiveComponent::GetWidgetComponent() const
{
	return WidgetComponent;
}

TSubclassOf<UIPInteractionWidget> UIPInteractiveComponent::GetInteractionWidgetClass() const
{
	return InteractionWidgetClass;
}

TSubclassOf<UUserWidget> UIPInteractiveComponent::GetIndicationWidgetClass() const
{
	return IndicationWidgetClass;
}

TSubclassOf<UUserWidget> UIPInteractiveComponent::GetIndicationBlockedWidgetClass() const
{
	return IndicationBlockedWidgetClass;
}

FText UIPInteractiveComponent::GetInteractiveName() const
{
	return InteractiveName;
}

FText UIPInteractiveComponent::GetInteractionDescription() const
{
	return InteractionDescription;
}

bool UIPInteractiveComponent::IsAutoInteractive() const
{
	return bIsAutoInteractive;
}

void UIPInteractiveComponent::StartInteractionPhase(AActor* InteractionInstigator)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}
	
	CurrentInteractor = InteractionInstigator;
	InteractiveState = FIPInteractiveState
	{
		.State = EIPInteractiveState::Busy,
		.InteractionCount = InteractiveState.InteractionCount,
	};

	OnRep_InteractiveState();
}

void UIPInteractiveComponent::EndInteractionPhase(EIPInteractiveState NextState)
{
	if (GetOwnerRole() != ROLE_Authority)
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
		GetOwner()->Destroy();
	}
	else
	{
		OnRep_InteractiveState();
	}

	CurrentInteractor = nullptr;
}

void UIPInteractiveComponent::NotifyStateChanged()
{
	IndicatedInteractors.RemoveAll([](const TWeakObjectPtr<UIPInteractorComponent>& Interactor)
{
	return !Interactor.IsValid();
});

	// Only notify indicated interactors because possible interactors will be updated on interactor's Tick
	for (const TWeakObjectPtr<UIPInteractorComponent>& Interactor : IndicatedInteractors)
	{
		Interactor->OnInteractiveStateChanged(this);
	}
}

void UIPInteractiveComponent::HandleInteractionTriggerBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (!OtherActor)
	{
		return;
	}

	// Try to get the Interactor component from the other actor
	auto* Interactor = OtherActor->GetComponentByClass<UIPInteractorComponent>();

	if (!Interactor)
	{
		return;
	}

	PossibleInteractors.Add(Interactor);
	Interactor->AddInteractive(this);
}

void UIPInteractiveComponent::HandleInteractionTriggerEndOverlap(
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
	auto* Interactor = OtherActor->GetComponentByClass<UIPInteractorComponent>();

	if (!Interactor)
	{
		return;
	}

	PossibleInteractors.Remove(Interactor);
	Interactor->RemoveInteractive(this);

	// End interaction if was interacting with this actor and he left the zone
	if (GetOwnerRole() == ROLE_Authority)
	{
		if (CurrentInteractor == OtherActor)
		{
			EndInteractionInput(OtherActor);	
		}
	}
}

void UIPInteractiveComponent::HandleIndicationTriggerBeginOverlap(
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
	auto* Interactor = OtherActor->GetComponentByClass<UIPInteractorComponent>();

	if (!Interactor)
	{
		return;
	}

	IndicatedInteractors.Add(Interactor);
	Interactor->AddInteractiveIndication(this);
}

void UIPInteractiveComponent::HandleIndicationTriggerEndOverlap(
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
	auto* Interactor = OtherActor->GetComponentByClass<UIPInteractorComponent>();

	if (!Interactor)
	{
		return;
	}

	IndicatedInteractors.Remove(Interactor);
	Interactor->RemoveInteractiveIndication(this);
}

void UIPInteractiveComponent::OnRep_InteractiveState()
{
	NotifyStateChanged();
	INTERACTION_HANDLER(DoInteractionFeedback, InteractiveState);
}
