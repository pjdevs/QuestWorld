// Copyright pjdevs. All Rights Reserved.


#include "IPStatefulComponent.h"

#include "IPStateHandler.h"
#include "Net/UnrealNetwork.h"


UIPStatefulComponent::UIPStatefulComponent()
	: bIsPersistent(false)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UIPStatefulComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UIPStatefulComponent, State);
}

EIPState UIPStatefulComponent::GetState() const
{
	return State;
}

void UIPStatefulComponent::SetState(EIPState NewState)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	const EIPState OldState = State;
	State = NewState;

	if (GetOwner()->IsNetMode(NM_DedicatedServer))
	{
		IIPStateHandler::Execute_OnStateChangedServer(GetOwner(), OldState, State);
	}
	else
	{
		IIPStateHandler::Execute_OnStateChangedServer(GetOwner(), OldState, State);
		IIPStateHandler::Execute_OnStateChangedClient(GetOwner(), OldState, State);
	}
}

void UIPStatefulComponent::OnRep_State(const EIPState& OldState)
{
	IIPStateHandler::Execute_OnStateChangedClient(GetOwner(), OldState, State);
}

bool UIPStatefulComponent::ShouldSkip_Implementation() const
{
	return !bIsPersistent;
}

void UIPStatefulComponent::SpudPostRestore_Implementation(const USpudState* SpudState)
{
	OnRep_State(State);
}
