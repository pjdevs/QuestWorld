// Copyright pjdevs. All Rights Reserved.


#include "Flow/Nodes/FlowNode_OnActorInteracted.h"
#include "IPInteractiveComponent.h"


UFlowNode_OnActorInteracted::UFlowNode_OnActorInteracted(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UFlowNode_OnActorInteracted::OnActorInteracted(AActor* InteractionInstigator)
{
	OnEventReceived();
}

void UFlowNode_OnActorInteracted::ObserveActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component)
{
	if (!RegisteredActors.Contains(Actor))
	{
		if (UIPInteractiveComponent* InteractiveComponent =	Actor->GetComponentByClass<UIPInteractiveComponent>())
		{
			RegisteredActors.Emplace(Actor, Component);
			InteractiveComponent->OnInteracted.AddDynamic(this, &UFlowNode_OnActorInteracted::OnActorInteracted);
		}
	}
}

void UFlowNode_OnActorInteracted::ForgetActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component)
{
	if (UIPInteractiveComponent* InteractiveComponent = Actor->GetComponentByClass<UIPInteractiveComponent>())
	{
		InteractiveComponent->OnInteracted.RemoveAll(this);
	}
}
