// Copyright pjdevs. All Rights Reserved.


#include "GaspAICharacter.h"
#include "GaspAbilitySystemComponent.h"
#include "GaspAttributeSet.h"

AGaspAICharacter::AGaspAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UGaspAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UGaspAttributeSet>("AttributeSet");
}

void AGaspAICharacter::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	GiveDefaultAbilities();
	InitDefaultAttributes();
	ApplyDefaultPermanentEffects();
}
