// Copyright pjdevs. All Rights Reserved.

#include "../Public/IPInteractorComponent.h"

#include "IPInteractionWidget.h"
#include "IPInteractive.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"


UIPInteractorComponent::UIPInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	InteractionDistance = 100.0f;
	InteractionTraceChannel = ECC_Visibility;
}

void UIPInteractorComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	
	HideInteractionWidget_Client_Implementation(MostRelevantActor);
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

void UIPInteractorComponent::Server_Interact_Implementation()
{
	Interact();
}

void UIPInteractorComponent::RecomputeInteractiveRelevancy()
{
	AActor* PreviousMostRelevantActor = MostRelevantActor;

	if (PossibleInteractives.IsEmpty())
	{
		MostRelevantActor = nullptr;
	}
	else
	{
		FVector EyesLocation;
		FRotator EyesRotation;
		GetOwner()->GetActorEyesViewPoint(EyesLocation, EyesRotation);

		const FVector LookDirection = EyesRotation.Vector().GetSafeNormal();

		auto ComputeInteractionScore = [&](const IIPInteractive& Target) -> TTuple<float, float>
		{
			FVector DirectionToTarget = Target.GetInteractiveLocation() - EyesLocation;
			const float DistanceToTarget = DirectionToTarget.Length();
			DirectionToTarget = DirectionToTarget.GetSafeNormal();
			const float AlignmentFromTarget = FMath::Max(0.f, LookDirection.Dot(DirectionToTarget));
			const float Score = AlignmentFromTarget / (1.f + DistanceToTarget * 0.01f);

			return MakeTuple(AlignmentFromTarget, Score);
		};

		AActor* NewMostRelevantActor = nullptr;
		float NewMostRelevantActorScore = 0.f;

		for (auto&& Interactive : PossibleInteractives)
		{
			const TTuple<float, float> ScoreTuple = ComputeInteractionScore(*Interactive);
			const float Alignment = ScoreTuple.Key;
			const float Score = ScoreTuple.Value;

			// TODO Compute real angle and expose value?
			if (Alignment <= 0.f)
			{
				continue;
			}

			if (Score > NewMostRelevantActorScore)
			{
				NewMostRelevantActor = Cast<AActor>(Interactive);
				NewMostRelevantActorScore = Score;
			}
		}

		MostRelevantActor = NewMostRelevantActor;
	}

	if (GetOwner()->HasAuthority() && MostRelevantActor != PreviousMostRelevantActor)
	{
		OnMostRelevantActorChanged(PreviousMostRelevantActor, MostRelevantActor);
	}
}

void UIPInteractorComponent::OnMostRelevantActorChanged(AActor* PreviousMostRelevantActor, AActor* NewMostRelevantActor)
{
	// Show hide interaction widgets
	HideInteractionWidget_Client(PreviousMostRelevantActor);

	if (NewMostRelevantActor)
	{
		ShowInteractionWidget_Client(NewMostRelevantActor);
	}
}

void UIPInteractorComponent::HideInteractionWidget_Client_Implementation(AActor* Interactive)
{
	if (const IIPInteractive* InteractiveActor = Cast<IIPInteractive>(Interactive))
	{
		UWidgetComponent* WidgetComponent = InteractiveActor->GetWorldSpaceInteractionWidgetSlot();

		if (WidgetComponent != nullptr)
		{
			WidgetComponent->SetWidget(nullptr);
		}
	}
	
	if (InteractionWidget)
	{
		InteractionWidget->RemoveFromParent();
	}

	InteractionWidget = nullptr;
}

void UIPInteractorComponent::ShowInteractionWidget_Client_Implementation(AActor* Interactive)
{
	const auto* InteractiveActor = Cast<IIPInteractive>(Interactive);

	if (!InteractiveActor)
	{
		return;
	}

	if (InteractiveActor->IsAutoInteractive())
	{
		return;
	}

	UWidgetComponent* WidgetComponent = InteractiveActor->GetWorldSpaceInteractionWidgetSlot();
	const bool bSupportsWorldInteractionWidget = WidgetComponent != nullptr;
	InteractionWidget = CreateWidget<UIPInteractionWidget>(GetWorld(), InteractionWidgetClass);

	if (!InteractionWidget)
	{
		return;
	}

	InteractionWidget->SetInteractionDescription(
		InteractionAction,
		InteractiveActor->GetInteractiveName(),
		InteractiveActor->GetInteractionDescription()
	);

	if (bSupportsWorldInteractionWidget)
	{
		WidgetComponent->SetWidget(InteractionWidget);
	}
	else
	{
		InteractionWidget->AddToViewport();
	}
}
