// Copyright pjdevs. All Rights Reserved.


#include "IPInteractiveComponent.h"
#include "IPInteractionHandler.h"
#include "IPInteractorComponent.h"
#include "Components/WidgetComponent.h"


UIPInteractiveComponent::UIPInteractiveComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	InteractiveName = FText::FromString("Interactive Actor");
	InteractionDescription = FText::FromString("Interact");
	bIsAutoInteractive = false;
}

void UIPInteractiveComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!InteractionTrigger)
	{
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
	}

	if (!IndicationTrigger)
	{
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
	}

	if (!InteractionWidget)
	{
		if (
			TArray<UActorComponent*> InteractionWidgets = GetOwner()->GetComponentsByTag(
				UWidgetComponent::StaticClass(),
				FName("InteractionWidget")
			);
			InteractionWidgets.Num() > 0
		)
		{
			InteractionWidget = Cast<UWidgetComponent>(InteractionWidgets[0]); 
		}
	}
	
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

	PurgeInvalidInteractors();

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

void UIPInteractiveComponent::SetInteractionTrigger(UPrimitiveComponent* InInteractionTrigger)
{
	InteractionTrigger = InInteractionTrigger;
}

void UIPInteractiveComponent::SetIndicationTrigger(UPrimitiveComponent* InIndicationTrigger)
{
	IndicationTrigger = InIndicationTrigger;
}

void UIPInteractiveComponent::SetInteractionWidget(UWidgetComponent* InInteractionWidget)
{
	InteractionWidget = InInteractionWidget;
}

void UIPInteractiveComponent::StartInteractionInput(AActor* InteractionInstigator)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	const FIPInteractionStatus InteractionStatus = GetInteractionStatusForActor(InteractionInstigator);

	if (!InteractionStatus.bCanStartInteraction)
	{
		// TODO Could make function OnInteractionFailed to track and/or play a "can't do that" feedback
		return;
	}

	IIPInteractionHandler::Execute_OnStartInteractionInput(GetOwner(), InteractionInstigator);
}

void UIPInteractiveComponent::EndInteractionInput(AActor* InteractionInstigator)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}
	
	if (InteractionInstigator != CurrentInteractorActor)
	{
		return;
	}

	IIPInteractionHandler::Execute_OnEndInteractionInput(GetOwner(), InteractionInstigator);
}

FIPInteractionStatus UIPInteractiveComponent::GetInteractionStatusForActor(AActor* InteractionInstigator) const
{
	return IIPInteractionHandler::Execute_GetInteractionStatusForActor(
		GetOwner(),
		InteractionInstigator
	);
}

FVector UIPInteractiveComponent::GetInteractiveLocation() const
{
	return InteractionWidget != nullptr
		? InteractionWidget->GetComponentLocation()
		: GetOwner()->GetActorLocation();
}

UWidgetComponent* UIPInteractiveComponent::GetWidgetComponent() const
{
	return InteractionWidget;
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
	
	CurrentInteractorActor = InteractionInstigator;
}

void UIPInteractiveComponent::EndInteractionPhase()
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	CurrentInteractorActor = nullptr;
}

void UIPInteractiveComponent::NotifyStatusChanged()
{
	PurgeInvalidInteractors();

	const TSet<TWeakObjectPtr<UIPInteractorComponent>> AllInteractors = PossibleInteractors.Union(IndicatedInteractors);
	
	for (const TWeakObjectPtr<UIPInteractorComponent>& Interactor : AllInteractors)
	{
		Interactor->OnInteractiveStatusChanged(this);
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
		if (CurrentInteractorActor == OtherActor)
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

void UIPInteractiveComponent::PurgeInvalidInteractors()
{
	for (auto It = PossibleInteractors.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
		}
	}

	for (auto It = IndicatedInteractors.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
		}
	}
}
