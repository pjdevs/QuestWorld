// Copyright pjdevs. All Rights Reserved.

#include "IPInteractiveActor.h"
#include "IPInteractiveComponent.h"
#include "IPStatefulComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"

AIPInteractiveActor::AIPInteractiveActor()
{
	bReplicates = true;

	InteractiveComponent = CreateDefaultSubobject<UIPInteractiveComponent>(TEXT("Interactive Component"));

	InteractionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Trigger"));
	InteractionTrigger->SetCollisionProfileName(TEXT("TriggerPawn"));
	InteractionTrigger->SetupAttachment(RootComponent);

	IndicationTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Indication Trigger"));
	IndicationTrigger->SetCollisionProfileName(TEXT("TriggerPawn"));
	IndicationTrigger->SetupAttachment(RootComponent);

	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction Widget"));
	InteractionWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionWidget->SetDrawAtDesiredSize(true);
	InteractionWidget->SetupAttachment(RootComponent);
}

void AIPInteractiveActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InteractiveComponent->SetInteractionTrigger(InteractionTrigger);
	InteractiveComponent->SetIndicationTrigger(IndicationTrigger);
	InteractiveComponent->SetInteractionWidget(InteractionWidget);
}

FIPInteractionStatus AIPInteractiveActor::GetInteractionStatusForActor_Implementation(AActor* InteractionInstigator)
{
	const EIPState CurrentState = StatefulComponent->GetState();
	FIPInteractionStatus InteractionStatus
	{
		.bCanStartInteraction = CurrentState == EIPState::Idle,
	};

	const FIPInteractionStatus& ExtraStatus = GetExtraInteractionStatusForActor(InteractionInstigator, CurrentState);
	InteractionStatus.bCanStartInteraction &= ExtraStatus.bCanStartInteraction;
	InteractionStatus.ReasonText = ExtraStatus.ReasonText;
	
	return InteractionStatus;
}

void AIPInteractiveActor::StartInteractionPhase(AActor* InteractionInstigator)
{
	if (!HasAuthority())
	{
		return;
	}
	
	InteractiveComponent->StartInteractionPhase(InteractionInstigator);
	StatefulComponent->SetState(EIPState::Activated);
}

void AIPInteractiveActor::EndInteractionPhase(EIPState NextState)
{
	if (!HasAuthority())
	{
		return;
	}

	StatefulComponent->SetState(NextState);
	InteractiveComponent->EndInteractionPhase();
}

FIPInteractionStatus AIPInteractiveActor::GetExtraInteractionStatusForActor_Implementation(
	AActor* InteractionInstigator,
	EIPState CurrentState
) const
{
	return FIPInteractionStatus { .bCanStartInteraction = true};
}

void AIPInteractiveActor::NotifyStatusChanged()
{
	InteractiveComponent->NotifyStatusChanged();
}
