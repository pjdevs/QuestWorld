// Copyright pjdevs. All Rights Reserved.


#include "GaspAICharacter.h"
#include "GaspAbilitySystemComponent.h"

AGaspAICharacter::AGaspAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UGaspAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

void AGaspAICharacter::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	GiveDefaultAbilities();
}
