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
		HideWidget_Client(MostRelevantActor.Get());
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

void UIPInteractorComponent::Interact()
{	
	if (!GetOwner()->HasAuthority())
	{
		Server_Interact();
		return;
	}
	
	auto* Interactive = Cast<IIPInteractive>(MostRelevantActor);
	
	if (!Interactive)
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

	if (!Interactive->CanBeInteracted(GetOwner()))
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
	RecomputeInteractiveRelevancy();
}

void UIPInteractorComponent::AddInteractiveIndication(IIPInteractive* Interactive)
{
	if (!Interactive)
	{
		return;
	}

	IndicatedInteractives.Add(Interactive);
	ShowIndicationWidget_Client(Cast<AActor>(Interactive));
}

void UIPInteractorComponent::RemoveInteractiveIndication(IIPInteractive* Interactive)
{
	if (!Interactive)
	{
		return;
	}

	IndicatedInteractives.Remove(Interactive);
	HideWidget_Client(Cast<AActor>(Interactive));
}

void UIPInteractorComponent::OnInteractiveStateChanged(IIPInteractive* Interactive)
{
	if (PossibleInteractives.Contains(Interactive))
	{
		if (!Interactive->CanBeInteracted(GetOwner()))
		{
			PossibleInteractives.Remove(Interactive);
		}

		RecomputeInteractiveRelevancy();
	}

	if (IndicatedInteractives.Contains(Interactive) && !PossibleInteractives.Contains(Interactive))
	{
		ShowIndicationWidget_Client(Cast<AActor>(Interactive));
	}
}

void UIPInteractorComponent::Server_Interact_Implementation()
{
	Interact();
}

void UIPInteractorComponent::RecomputeInteractiveRelevancy()
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

	if (GetOwner()->HasAuthority() && MostRelevantActor != PreviousMostRelevantInteractive)
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
	FInteractionScore NewMostRelevantInteractiveScore
	{
		.InteractionScore = 0.0f,
	};

	for (auto&& Interactive : PossibleInteractives)
	{
		const FInteractionScore Score = ComputeInteractionScore(*Interactive, EyesLocation, LookDirection);

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
		ShowIndicationWidget_Client(PreviousMostRelevantActor);
	}

	if (NewMostRelevantActor)
	{
		if (const IIPInteractive* Interactive = Cast<IIPInteractive>(NewMostRelevantActor))
		{
			if (Interactive->IsAutoInteractive())
			{
				Interact();	
			}
			else
			{
				ShowInteractionWidget_Client(NewMostRelevantActor);
			}
		}
	}
}

FInteractionScore UIPInteractorComponent::ComputeInteractionScore(
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

	return FInteractionScore
	{
		.InteractionScore = Score,
		.AngleFromTarget = AngleFromTarget,
		.DistanceFromTarget = DistanceToTarget,
	};
}

void UIPInteractorComponent::ShowInteractionWidget_Client_Implementation(AActor* InteractiveActor)
{
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
		Interactive->GetInteractionDescription()
	);

	WidgetComponent->SetWidget(WidgetInstance);
}

void UIPInteractorComponent::ShowIndicationWidget_Client_Implementation(AActor* InteractiveActor)
{
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

	const bool bCanBeInteracted = Interactive->CanBeInteracted(GetOwner());
	const TSubclassOf<UUserWidget> WidgetClass = bCanBeInteracted
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

void UIPInteractorComponent::HideWidget_Client_Implementation(AActor* InteractiveActor)
{
	if (const IIPInteractive* Interactive = Cast<IIPInteractive>(InteractiveActor))
	{
		if (UWidgetComponent* WidgetComponent = Interactive->GetWidgetComponent())
		{
			if (UUserWidget* WidgetInside = WidgetComponent->GetWidget())
			{
				WidgetInside->RemoveFromParent();
			}

			WidgetComponent->SetWidget(nullptr);
		}
	}
}
