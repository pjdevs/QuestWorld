// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/IPInteractiveActor.h"
#include "Net/UnrealNetwork.h"

AIPInteractiveActor::AIPInteractiveActor()
{
	bReplicates = true;

	InteractionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Intraction Trigger"));
	InteractionTrigger->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = InteractionTrigger;

	State = EIPInteractiveState::Ready;
	InteractMultipleTimes = true;
	InteractionDescription = "Interact";
	AutoInteract = false;
}

void AIPInteractiveActor::HandleTriggerBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult)
{
	if (!Actor) return;

	auto Interactor = Cast<UIPInteractorComponent>(Actor->GetComponentByClass(UIPInteractorComponent::StaticClass()));
	if (!Interactor) return;

	PossibleInteractors.Add(Interactor);
	Interactor->AddInteractive(this);

	if (AutoInteract)
	{
		Interactor->Interact();
	}
}

void AIPInteractiveActor::HandleTriggerEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I)
{
	if (!Actor) return;
	auto Interactor = Cast<UIPInteractorComponent>(Actor->GetComponentByClass(UIPInteractorComponent::StaticClass()));
	if (!Interactor) return;
	PossibleInteractors.Remove(Interactor);
	Interactor->RemoveInteractive(this);
}

void AIPInteractiveActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &AIPInteractiveActor::HandleTriggerBeginOverlap);
	InteractionTrigger->OnComponentEndOverlap.AddDynamic(this, &AIPInteractiveActor::HandleTriggerEndOverlap);
}

void AIPInteractiveActor::Interact(AActor* InteractionInstigator)
{
	if (!HasAuthority()) return;
	if (!CanBeInteracted(InteractionInstigator)) return;

	DoInteract(InteractionInstigator);
	BP_DoInteract(InteractionInstigator);
	
	if (InteractMultipleTimes)
		State = State == EIPInteractiveState::Interacted ? EIPInteractiveState::Ready : EIPInteractiveState::Interacted;
	else
		State = EIPInteractiveState::Interacted;
	
	OnRep_State();
}

bool AIPInteractiveActor::CanBeInteracted(AActor* InteractionInstigator)
{
	if (InteractMultipleTimes)
		return true;
	else
		return State == EIPInteractiveState::Ready;
}

FString AIPInteractiveActor::GetInteractionDescription() const
{
	return InteractionDescription;
}

bool AIPInteractiveActor::IsAutoInteractive() const
{
	return AutoInteract;
}

void AIPInteractiveActor::OnRep_State()
{
	if (State == EIPInteractiveState::Interacted || InteractMultipleTimes)
	{
		for (auto* Interactor : PossibleInteractors)
		{
			if (!InteractMultipleTimes || !CanBeInteracted(Interactor->GetOwner()))
			{
				Interactor->RemoveInteractive(this);
			}
		}
		
		DoFeedback();
		BP_DoFeedback();
	}
}

void AIPInteractiveActor::DoInteract(AActor* InteractionInstigator)
{
}

void AIPInteractiveActor::BP_DoInteract_Implementation(AActor* InteractionInstigator)
{
}

void AIPInteractiveActor::DoFeedback()
{
}

void AIPInteractiveActor::BP_DoFeedback_Implementation()
{
}

void AIPInteractiveActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AIPInteractiveActor, State);
}
