// Copyright pjdevs. All Rights Reserved.


#include "Ability/GameplayAbility_Interact.h"
#include "IPInteractorComponent.h"
#include "Interactive/AbilityInteractiveActor.h"


UGameplayAbility_Interact::UGameplayAbility_Interact()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnlyTermination;
}

void UGameplayAbility_Interact::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		return;
	}
	
	if (!ActorInfo->AvatarActor.IsValid())
	{
		return;
	}

	UIPInteractorComponent* InteractorComponent = ActorInfo->AvatarActor.Get()->GetComponentByClass<UIPInteractorComponent>();

	if (!InteractorComponent)
	{
		return;
	}

	const TWeakObjectPtr<AActor> MostRelevantActor = InteractorComponent->GetMostRelevantActor();

	if (!MostRelevantActor.IsValid())
	{
		return;
	}

	AAbilityInteractiveActor* InteractiveActor = Cast<AAbilityInteractiveActor>(MostRelevantActor.Get());

	if (!InteractiveActor)
	{
		return;
	}

	InteractiveActor->OnPreAbilityInteract(GetCurrentAbilitySpecHandle());
	InteractorComponent->TryInteract();
	InteractiveActor->OnPostAbilityInteract();

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
