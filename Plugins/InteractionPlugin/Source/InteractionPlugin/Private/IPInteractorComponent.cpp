// Copyright pjdevs. All Rights Reserved.

#include "IPInteractorComponent.h"
#include "IPInteractionWidget.h"
#include "IPInteractive.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"


UIPInteractorComponent::UIPInteractorComponent()
	: MaxInteractionDistance(1000.f), MaxInteractionAngleDegrees(70.f)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UIPInteractorComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);

	if (MostRelevantActor.IsValid())
	{
		HideWidgetClient(MostRelevantActor.Get());
	}
}

void UIPInteractorComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
)
{
	if (PossibleInteractives.IsEmpty())
	{
		return;
	}

	RecomputeInteractiveRelevancy();	
}

void UIPInteractorComponent::TryInteract()
{
	IIPInteractive* Interactive = Cast<IIPInteractive>(MostRelevantActor);

	if (!Interactive)
	{
		return;
	}

	// check that we can interact! (on client and server)
	const FIPInteractionStatus InteractionStatus = Interactive->GetInteractionStatus(GetOwner());

	if (!InteractionStatus.bCanBeInteracted)
	{
		return;
	}

	// if we are on client call this method on server 
	if (GetOwnerRole() != ROLE_Authority)
	{
		Server_TryInteract();
		return;
	}

	// check on server if we really are in range to interact (is it really possible that it is not the case???)
	if (!PossibleInteractives.Contains(Interactive))
	{
		return;
	}

	Interactive->Interact(GetOwner());
}

void UIPInteractorComponent::AddInteractive(IIPInteractive* Interactive)
{
	if (!Interactive)
	{
		return;
	}

	if (PossibleInteractives.Contains(Interactive))
	{
		return;
	}

	PossibleInteractives.Add(Interactive);
	RecomputeInteractiveRelevancy();
}

void UIPInteractorComponent::RemoveInteractive(IIPInteractive* Interactive)
{
	if (!Interactive)
	{
		return;
	}

	PossibleInteractives.Remove(Interactive);

	// Interactive could be removed because it destroyed, so check that we can do widget stuff on it before
	const AActor* InteractiveActor = Cast<AActor>(Interactive);
	
	if (IsValid(InteractiveActor))
	{
		RecomputeInteractiveRelevancy();
	}
	else if (MostRelevantActor == InteractiveActor)
	{
		MostRelevantActor = nullptr;
	}
}

void UIPInteractorComponent::AddInteractiveIndication(IIPInteractive* Interactive)
{
	if (!Interactive)
	{
		return;
	}

	IndicatedInteractives.Add(Interactive);
	ShowIndicationWidgetClient(Cast<AActor>(Interactive));
}

void UIPInteractorComponent::RemoveInteractiveIndication(IIPInteractive* Interactive)
{
	if (!Interactive)
	{
		return;
	}

	IndicatedInteractives.Remove(Interactive);

	// Interactive could be removed because it destroyed, so check that we can do widget stuff on it before
	AActor* InteractiveActor = Cast<AActor>(Interactive);
	
	if (IsValid(InteractiveActor))
	{
		HideWidgetClient(InteractiveActor);
	}
}

void UIPInteractorComponent::OnInteractiveStateChanged(IIPInteractive* Interactive)
{
	if (!PossibleInteractives.Contains(Interactive) && IndicatedInteractives.Contains(Interactive))
	{
		ShowIndicationWidgetClient(Cast<AActor>(Interactive));
	}
	
	RecomputeInteractiveRelevancy(true);
}

void UIPInteractorComponent::Server_TryInteract_Implementation()
{
	TryInteract();
}

void UIPInteractorComponent::RecomputeInteractiveRelevancy(bool bForceRefresh)
{
	AActor* PreviousMostRelevantInteractive = MostRelevantActor.IsValid()
		? MostRelevantActor.Get()
		: nullptr;

	if (PossibleInteractives.IsEmpty())
	{
		MostRelevantActor = nullptr;
	}
	else
	{
		PurgePossibleInteractives();
		MostRelevantActor = FindNewMostRelevantActor();
	}

	if (MostRelevantActor != PreviousMostRelevantInteractive || bForceRefresh)
	{
		OnMostRelevantInteractiveChanged(PreviousMostRelevantInteractive, MostRelevantActor.Get());
	}
}

void UIPInteractorComponent::PurgePossibleInteractives()
{
	PossibleInteractives.RemoveAll([](const TWeakInterfacePtr<IIPInteractive>& Ptr)
	{
		return !Ptr.IsValid();
	});
}

AActor* UIPInteractorComponent::FindNewMostRelevantActor() const
{
	FVector EyesLocation;
	FRotator EyesRotation;
	GetOwner()->GetActorEyesViewPoint(EyesLocation, EyesRotation);
	const FVector LookDirection = EyesRotation.Vector().GetSafeNormal();
		
	AActor* NewMostRelevantActor = nullptr;
	FIPInteractionScore NewMostRelevantInteractiveScore
	{
		.InteractionScore = 0.0f,
	};

	for (auto&& Interactive : PossibleInteractives)
	{
		const FIPInteractionScore Score = ComputeInteractionScore(*Interactive, EyesLocation, LookDirection);

		// TODO Compute real angle and expose value?
		if (Score.AngleFromTarget >= MaxInteractionAngleDegrees || Score.DistanceFromTarget >= MaxInteractionDistance)
		{
			continue;
		}

		if (Score.InteractionScore > NewMostRelevantInteractiveScore.InteractionScore)
		{
			NewMostRelevantActor = Cast<AActor>(Interactive.Get());
			NewMostRelevantInteractiveScore = Score;
		}
	}

	return NewMostRelevantActor;
}

void UIPInteractorComponent::OnMostRelevantInteractiveChanged(
	AActor* PreviousMostRelevantActor,
	AActor* NewMostRelevantActor
)
{
	// Show hide interaction widgets
	if (PreviousMostRelevantActor)
	{
		if (IndicatedInteractives.Contains(Cast<IIPInteractive>(PreviousMostRelevantActor)))
		{
			ShowIndicationWidgetClient(PreviousMostRelevantActor);
		}
		else
		{
			HideWidgetClient(PreviousMostRelevantActor);
		}
	}

	if (NewMostRelevantActor)
	{
		if (const IIPInteractive* Interactive = Cast<IIPInteractive>(NewMostRelevantActor))
		{
			if (Interactive->IsAutoInteractive() && GetOwnerRole() == ROLE_Authority)
			{
				TryInteract();	
			}
			else
			{
				ShowInteractionWidgetClient(NewMostRelevantActor);
			}
		}
	}
}

FIPInteractionScore UIPInteractorComponent::ComputeInteractionScore(
	const IIPInteractive& Target,
	const FVector& EyesLocation,
	const FVector& LookDirection
)
{
	const FVector TargetLocation = Target.GetInteractiveLocation();
	FVector DirectionToTarget = TargetLocation - EyesLocation;
	const float DistanceToTarget = DirectionToTarget.Length();
	DirectionToTarget = DirectionToTarget.GetSafeNormal();

	const float AlignmentFromTarget = FMath::Max(0.f, LookDirection.Dot(DirectionToTarget));
	const float AngleFromTargetRadians = FMath::Acos(AlignmentFromTarget);
	const float AngleFromTarget = FMath::RadiansToDegrees(AngleFromTargetRadians);

	const float Score = AlignmentFromTarget / (1.f + DistanceToTarget * 0.01f);

	return FIPInteractionScore
	{
		.InteractionScore = Score,
		.AngleFromTarget = AngleFromTarget,
		.DistanceFromTarget = DistanceToTarget,
	};
}

void UIPInteractorComponent::ShowInteractionWidgetClient(AActor* InteractiveActor)
{
	if (!IsLocal())
	{
		return;
	}

	const auto* Interactive = Cast<IIPInteractive>(InteractiveActor);

	if (!InteractiveActor)
	{
		return;
	}

	if (Interactive->IsAutoInteractive())
	{
		return;
	}

	UWidgetComponent* WidgetComponent = Interactive->GetWidgetComponent();

	if (!WidgetComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No WidgetComponent found on interactive."));
		return;
	}
	
	UIPInteractionWidget* WidgetInstance = CreateWidget<UIPInteractionWidget>(
		GetWorld(),
		Interactive->GetInteractionWidgetClass()
	);

	if (!WidgetInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create interaction widget."));
		return;
	}

	WidgetInstance->SetInteractionDescription(
		InteractionAction,
		Interactive->GetInteractiveName(),
		Interactive->GetInteractionDescription(),
		Interactive->GetInteractionStatus(GetOwner())
	);

	WidgetComponent->SetWidget(WidgetInstance);
}

void UIPInteractorComponent::ShowIndicationWidgetClient(AActor* InteractiveActor)
{
	if (!IsLocal())
	{
		return;
	}

	const auto* Interactive = Cast<IIPInteractive>(InteractiveActor);

	if (!InteractiveActor)
	{
		return;
	}

	UWidgetComponent* WidgetComponent = Interactive->GetWidgetComponent();

	if (!WidgetComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No WidgetComponent found on interactive."));
		return;
	}

	const FIPInteractionStatus InteractionStatus = Interactive->GetInteractionStatus(GetOwner());
	const TSubclassOf<UUserWidget> WidgetClass = InteractionStatus.bCanBeInteracted
		? Interactive->GetIndicationWidgetClass()
		: Interactive->GetIndicationBlockedWidgetClass();
	UUserWidget* WidgetInstance = CreateWidget(GetWorld(), WidgetClass);

	if (!WidgetInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create indication widget."));
		return;
	}

	WidgetComponent->SetWidget(WidgetInstance);
}

void UIPInteractorComponent::HideWidgetClient(AActor* InteractiveActor)
{
	if (!IsLocal())
	{
		return;
	}

	if (const IIPInteractive* Interactive = Cast<IIPInteractive>(InteractiveActor))
	{
		if (UWidgetComponent* WidgetComponent = Interactive->GetWidgetComponent())
		{
			WidgetComponent->SetWidget(nullptr);
		}
	}
}

bool UIPInteractorComponent::IsLocal() const
{
	if (const APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		return OwnerPawn->IsLocallyControlled();
	}

	return false;
}
