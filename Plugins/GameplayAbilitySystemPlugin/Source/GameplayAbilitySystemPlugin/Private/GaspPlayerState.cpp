// Copyright pjdevs. All Rights Reserved.


#include "GaspPlayerState.h"
#include "GaspAbilitySystemComponent.h"


AGaspPlayerState::AGaspPlayerState()
{
	SetNetUpdateFrequency(100);

	AbilitySystemComponent = CreateDefaultSubobject<UGaspAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* AGaspPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
