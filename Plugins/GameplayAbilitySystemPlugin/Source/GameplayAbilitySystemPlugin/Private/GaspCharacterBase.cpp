// Copyright pjdevs. All Rights Reserved.


#include "GaspCharacterBase.h"
#include "GaspAbilitySystemComponent.h"


AGaspCharacterBase::AGaspCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

UAbilitySystemComponent* AGaspCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGaspCharacterBase::GiveDefaultAbilities()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	if (!HasAuthority())
	{
		return;
	}

	for (const TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
	{
		const FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}
