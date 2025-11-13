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

void AGaspPlayerState::GiveGrantedAbilities()
{
	if (!HasAuthority())
	{
		return;
	}

	for (const TSubclassOf<UGameplayAbility>& AbilityClass : GrantedAbilities)
	{
		GiveAbilityToPlayer(AbilityClass);
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
	GiveAbilityToPlayer(AbilityClass);
}

void AGaspPlayerState::SpudPostRestore_Implementation(const USpudState* State)
{
	GiveGrantedAbilities();
}

void AGaspPlayerState::GiveAbilityToPlayer(TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (!HasAuthority())
	{
		return;
	}

	const FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, INDEX_NONE, this);
	AbilitySystemComponent->GiveAbility(AbilitySpec);
}
