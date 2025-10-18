// Copyright pjdevs. All Rights Reserved.


#include "GaspPlayerState.h"
#include "GaspAbilitySystemComponent.h"
#include "GaspAttributeSet.h"


AGaspPlayerState::AGaspPlayerState()
{
	SetNetUpdateFrequency(100);

	AbilitySystemComponent = CreateDefaultSubobject<UGaspAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UGaspAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AGaspPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
