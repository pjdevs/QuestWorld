// Copyright pjdevs. All Rights Reserved.


#include "GaspCharacterBase.h"
#include "GameplayEffect.h"
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

void AGaspCharacterBase::InitDefaultAttributes()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		DefaultAttributeEffect,
		1.0f,
		EffectContextHandle
	);

	if (EffectSpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}

void AGaspCharacterBase::ApplyDefaultPermanentEffects()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	for (const TSubclassOf<UGameplayEffect>& EffectClass : DefaultPermanentEffects)
	{
		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
			EffectClass,
			1.0f,
			EffectContextHandle
		);

		if (EffectSpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
	}
}
