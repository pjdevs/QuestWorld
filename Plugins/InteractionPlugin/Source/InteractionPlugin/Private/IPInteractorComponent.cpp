// Copyright pjdevs. All Rights Reserved.

#include "IPInteractorComponent.h"
#include "IPInteractionWidget.h"
#include "IPInteractiveComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"


UIPInteractorComponent::UIPInteractorComponent()
	: MaxInteractionDistance(1000.f), MaxInteractionAngleDegrees(70.f)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UIPInteractorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocal())
	{
		SetComponentTickEnabled(false);
		SetActive(false);
	}
}

void UIPInteractorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (MostRelevantInteractive.IsValid())
	{
		HideWidgetClient(MostRelevantInteractive.Get());
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

void UIPInteractorComponent::TryStartInteractionInput()
{
	if (!MostRelevantInteractive.IsValid())
	{
		return;
	}

	// check that we can start interact! (on client and server)
	const FIPInteractionStatus InteractionStatus = MostRelevantInteractive->GetInteractionStatusForActor(GetOwner());

	if (!InteractionStatus.bCanStartInteraction)
	{
		return;
	}

	// if we are on client call this method on server 
	if (GetOwnerRole() != ROLE_Authority)
	{
		Server_TryStartInteractionInput();
		return;
	}

	// check on server if we really are in range to interact (is it really possible that it is not the case???)
	if (!PossibleInteractives.Contains(MostRelevantInteractive))
	{
		return;
	}

	MostRelevantInteractive->StartInteractionInput(GetOwner());
}

void UIPInteractorComponent::TryEndInteractionInput()
{
	if (!MostRelevantInteractive.IsValid())
	{
		return;
	}

	// if we are on client call this method on server 
	if (GetOwnerRole() != ROLE_Authority)
	{
		Server_TryEndInteractionInput();
		return;
	}

	// check on server if we really are in range to interact (is it really possible that it is not the case???)
	if (!PossibleInteractives.Contains(MostRelevantInteractive))
	{
		return;
	}

	MostRelevantInteractive->EndInteractionInput(GetOwner());
}

void UIPInteractorComponent::AddInteractive(UIPInteractiveComponent* Interactive)
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

void UIPInteractorComponent::RemoveInteractive(UIPInteractiveComponent* Interactive)
{
	const UWorld* World = GetWorld();

	// If world tearing down nothing need to be done as everything will get destroyed
	if (!IsValid(World) || World->bIsTearingDown)
	{
		return;
	}

	PossibleInteractives.Remove(Interactive);

	// Interactive could be removed because it destroyed, so check that we can do widget stuff on it before
	if (IsValid(Interactive))
	{
		RecomputeInteractiveRelevancy();
	}
	else if (MostRelevantInteractive == Interactive)
	{
		MostRelevantInteractive = nullptr;
	}
}

void UIPInteractorComponent::AddInteractiveIndication(UIPInteractiveComponent* Interactive)
{
	if (!Interactive)
	{
		return;
	}

	IndicatedInteractives.Add(Interactive);
	ShowIndicationWidgetClient(Interactive);
}

void UIPInteractorComponent::RemoveInteractiveIndication(UIPInteractiveComponent* Interactive)
{
	const UWorld* World = GetWorld();

	if (!IsValid(World) || World->bIsTearingDown)
	{
		return;
	}

	if (!Interactive)
	{
		return;
	}

	IndicatedInteractives.Remove(Interactive);

	// Interactive could be removed because it destroyed, so check that we can do widget stuff on it before
	if (IsValid(Interactive))
	{
		HideWidgetClient(Interactive);
	}
}

void UIPInteractorComponent::OnInteractiveStatusChanged(UIPInteractiveComponent* Interactive)
{
	if (!PossibleInteractives.Contains(Interactive) && IndicatedInteractives.Contains(Interactive))
	{
		ShowIndicationWidgetClient(Interactive);
	}
	
	RecomputeInteractiveRelevancy(true);
}

TWeakObjectPtr<UIPInteractiveComponent> UIPInteractorComponent::GetMostRelevantInteractive() const
{
	return MostRelevantInteractive;
}

void UIPInteractorComponent::Server_TryStartInteractionInput_Implementation()
{
	TryStartInteractionInput();
}

void UIPInteractorComponent::RecomputeInteractiveRelevancy(bool bForceRefresh)
{
	UIPInteractiveComponent* PreviousMostRelevantInteractive = MostRelevantInteractive.IsValid()
		? MostRelevantInteractive.Get()
		: nullptr;

	if (PossibleInteractives.IsEmpty())
	{
		MostRelevantInteractive = nullptr;
	}
	else
	{
		PurgeInvalidInteractives();
		MostRelevantInteractive = FindNewMostRelevantActor();
	}

	if (MostRelevantInteractive != PreviousMostRelevantInteractive || bForceRefresh)
	{
		OnMostRelevantInteractiveChanged(PreviousMostRelevantInteractive, MostRelevantInteractive.Get());
	}
}

void UIPInteractorComponent::PurgeInvalidInteractives()
{
	for (auto It = PossibleInteractives.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
		}
	}

	for (auto It = IndicatedInteractives.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
		}
	}
}

UIPInteractiveComponent* UIPInteractorComponent::FindNewMostRelevantActor() const
{
	FVector EyesLocation;
	FRotator EyesRotation;
	GetOwner()->GetActorEyesViewPoint(EyesLocation, EyesRotation);
	const FVector LookDirection = EyesRotation.Vector().GetSafeNormal();
		
	UIPInteractiveComponent* NewMostRelevantInteractive = nullptr;
	FIPInteractionScore NewMostRelevantInteractiveScore
	{
		.InteractionScore = 0.0f,
	};

	for (const TWeakObjectPtr<UIPInteractiveComponent>& Interactive : PossibleInteractives)
	{
		const FIPInteractionScore Score = ComputeInteractionScore(*Interactive, EyesLocation, LookDirection);

		if (Score.AngleFromTarget >= MaxInteractionAngleDegrees || Score.DistanceFromTarget >= MaxInteractionDistance)
		{
			continue;
		}

		if (Score.InteractionScore > NewMostRelevantInteractiveScore.InteractionScore)
		{
			NewMostRelevantInteractive = Interactive.Get();
			NewMostRelevantInteractiveScore = Score;
		}
	}

	return NewMostRelevantInteractive;
}

void UIPInteractorComponent::OnMostRelevantInteractiveChanged(
	UIPInteractiveComponent* PreviousMostRelevantInteractive,
	UIPInteractiveComponent* NewMostRelevantInteractive
)
{
	// Show hide interaction widgets
	if (PreviousMostRelevantInteractive)
	{
		if (IndicatedInteractives.Contains(PreviousMostRelevantInteractive))
		{
			ShowIndicationWidgetClient(PreviousMostRelevantInteractive);
		}
		else
		{
			HideWidgetClient(PreviousMostRelevantInteractive);
		}
	}

	if (NewMostRelevantInteractive)
	{
		if (NewMostRelevantInteractive->IsAutoInteractive() && GetOwnerRole() == ROLE_Authority)
		{
			TryStartInteractionInput();	
		}
		else
		{
			ShowInteractionWidgetClient(NewMostRelevantInteractive);
		}
	}
}

FIPInteractionScore UIPInteractorComponent::ComputeInteractionScore(
	const UIPInteractiveComponent& Target,
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

void UIPInteractorComponent::Server_TryEndInteractionInput_Implementation()
{
	TryEndInteractionInput();
}

void UIPInteractorComponent::ShowInteractionWidgetClient(UIPInteractiveComponent* Interactive)
{
	if (!IsLocal())
	{
		return;
	}

	if (!Interactive)
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
		Interactive->GetInteractionStatusForActor(GetOwner())
	);

	WidgetComponent->SetWidget(WidgetInstance);
}

void UIPInteractorComponent::ShowIndicationWidgetClient(UIPInteractiveComponent* Interactive)
{
	if (!IsLocal())
	{
		return;
	}

	if (!Interactive)
	{
		return;
	}

	UWidgetComponent* WidgetComponent = Interactive->GetWidgetComponent();

	if (!WidgetComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No WidgetComponent found on interactive."));
		return;
	}

	const FIPInteractionStatus InteractionStatus = Interactive->GetInteractionStatusForActor(GetOwner());
	const TSubclassOf<UUserWidget> WidgetClass = InteractionStatus.bCanStartInteraction
		? Interactive->GetIndicationWidgetClass()
		: Interactive->GetIndicationBlockedWidgetClass();

	UWorld* World = GetWorld();
	
	if (!IsValid(World))
	{
		return;
	}
	
	UUserWidget* WidgetInstance = CreateWidget(World, WidgetClass);

	if (!WidgetInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create indication widget."));
		return;
	}

	WidgetComponent->SetWidget(WidgetInstance);
}

void UIPInteractorComponent::HideWidgetClient(UIPInteractiveComponent* Interactive)
{
	if (!IsLocal())
	{
		return;
	}
	
	if (UWidgetComponent* WidgetComponent = Interactive->GetWidgetComponent())
	{
		WidgetComponent->SetWidget(nullptr);
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
