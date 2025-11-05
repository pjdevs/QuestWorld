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

void AGaspPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	if (AGaspPlayerState* NewGaspPlayerState = Cast<AGaspPlayerState>(PlayerState))
	{
		NewGaspPlayerState->GrantedAbilities = GrantedAbilities;
	}
}

void AGaspPlayerState::GiveGrantedAbilities()
{
	if (!HasAuthority())
	{
		return;
	}

	for (const TSubclassOf<UGameplayAbility>& AbilityClass : GrantedAbilities)
	{
		const FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, INDEX_NONE, this);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

UAbilitySystemComponent* AGaspPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGaspPlayerState::GrantAbility(TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (!HasAuthority())
	{
		return;
	}
	
	GrantedAbilities.Add(AbilityClass);

	const FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, INDEX_NONE, this);
	AbilitySystemComponent->GiveAbility(AbilitySpec);
}
