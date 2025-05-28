// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/IPInteractorComponent.h"

#include "IPInteractionWidget.h"
#include "../Public/IPInteractive.h"
#include "Blueprint/UserWidget.h"


UIPInteractorComponent::UIPInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	InteractionDistance = 100.0f;
	InteractionTraceChannel = ECC_Visibility;
}

void UIPInteractorComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	
	HideInteractionWidget_Client_Implementation();
}

void UIPInteractorComponent::Interact()
{	
	if (!GetOwner()->HasAuthority())
	{
		Server_Interact();
		return;
	}
	
	auto* Interactive = Cast<IIPInteractive>(MostRelevantActor);
	
	if (!Interactive) return;

	Interactive->Interact(GetOwner());
}

void UIPInteractorComponent::AddInteractive(IIPInteractive* Interactive)
{
	if (!Interactive) return;
	if (!Interactive->CanBeInteracted(GetOwner())) return;
	if (PossibleInteractives.Contains(Interactive)) return;

	PossibleInteractives.Add(Interactive);
	RecomputeInteractiveRelevancy();
}

void UIPInteractorComponent::RemoveInteractive(IIPInteractive* Interactive)
{
	if (!Interactive) return;
	PossibleInteractives.Remove(Interactive);
	RecomputeInteractiveRelevancy();
}

void UIPInteractorComponent::SetInteractionTraceDelegate(FInteractionTraceDelegate Delegate)
{
	InteractionTraceDelegate = Delegate;
}

void UIPInteractorComponent::Server_Interact_Implementation()
{
	Interact();
}

void UIPInteractorComponent::RecomputeInteractiveRelevancy()
{
	if (!ensureMsgf(InteractionTraceDelegate.IsBound(), TEXT("InteractionLineSweepDelegate was not boud. Did you forget to call SetInteractionLineSweepDelegate?"))) return;

	const auto* PreviousMostRelevantActor = MostRelevantActor;
	MostRelevantActor = PossibleInteractives.IsEmpty() ? nullptr : Cast<AActor>(PossibleInteractives[0]);
	
	if (TArray<FHitResult> Results; InteractionTraceDelegate.Execute(InteractionDistance, InteractionTraceChannel, Results))
	{
		for (const auto& Result : Results)
		{
			if (!Result.GetActor()) continue;
			if (!Result.GetActor()->Implements<UIPInteractive>()) break; // Make walls etc block interaction
			const auto* InteractiveActorHit = Cast<IIPInteractive>(Result.GetActor());
			const bool ExistInList = PossibleInteractives.ContainsByPredicate([&](const IIPInteractive* Interactive)
			{
				return Interactive == InteractiveActorHit;
			});

			if (!ExistInList) break; // Take only first it into account
			MostRelevantActor = Result.GetActor();

			break; // Take only first it into account
		}
	}

	if (GetOwner()->HasAuthority() && MostRelevantActor != PreviousMostRelevantActor)
	{
		HideInteractionWidget_Client();

		if (MostRelevantActor)
		{
			ShowInteractionWidget_Client(MostRelevantActor);
		}
	}

	PossibleInteractives.Sort([&](const IIPInteractive& A, const IIPInteractive& B)
	{
		const auto OwnerLocation = GetOwner()->GetActorLocation();
		return FVector::DistSquared(OwnerLocation, A.GetInteractiveLocation())
			< FVector::DistSquared(OwnerLocation, B.GetInteractiveLocation());
	});
}

void UIPInteractorComponent::HideInteractionWidget_Client_Implementation()
{
	if (InteractionWidget)
	{
		InteractionWidget->RemoveFromParent();
	}

	InteractionWidget = nullptr;
}

void UIPInteractorComponent::ShowInteractionWidget_Client_Implementation(AActor* Interactive)
{
	const auto* InteractiveActor = Cast<IIPInteractive>(Interactive);
	if (!InteractiveActor) return;
	if (InteractiveActor->IsAutoInteractive()) return;

	InteractionWidget = CreateWidget<UIPInteractionWidget>(GetWorld(), InteractionWidgetClass);
	InteractionWidget->SetInteractionDescription(InteractiveActor->GetInteractionDescription());
	
	if (InteractionWidget)
	{
		InteractionWidget->AddToViewport();
	}
}
