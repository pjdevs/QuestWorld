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
		NewGaspPlayerState->LoadFromSave(WriteToSave());
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
		GiveAbilityToPlayer(AbilityClass);
	}
}

FGaspPlayerSaveData AGaspPlayerState::WriteToSave() const
{
	return FGaspPlayerSaveData
	{
		.GrantedAbilities = GrantedAbilities
	};
}

void AGaspPlayerState::LoadFromSave(const FGaspPlayerSaveData& SaveData)
{
	// For now does not grant abilities in addition because this will be done at pawn init but could see later
	GrantedAbilities = SaveData.GrantedAbilities;
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

void AGaspPlayerState::GiveAbilityToPlayer(TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (!HasAuthority())
	{
		return;
	}

	const FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, INDEX_NONE, this);
	AbilitySystemComponent->GiveAbility(AbilitySpec);
}
