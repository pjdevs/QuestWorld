// Copyright pjdevs. All Rights Reserved.


#include "GaspPlayerCharacter.h"
#include "GaspAbilitySystemComponent.h"
#include "GaspPlayerState.h"


AGaspPlayerCharacter::AGaspPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGaspPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilitySystemComponent();
	GiveDefaultAbilities(); // only give abilities on the server
	InitDefaultAttributes();
	ApplyDefaultPermanentEffects(); // only apply permanent effects on server?
}

void AGaspPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilitySystemComponent();
	InitDefaultAttributes();
}

void AGaspPlayerCharacter::InitAbilitySystemComponent()
{
	if (AGaspPlayerState* GaspPlayerState = Cast<AGaspPlayerState>(GetPlayerState()))
	{
		AbilitySystemComponent = CastChecked<UGaspAbilitySystemComponent>(GaspPlayerState->GetAbilitySystemComponent());
		AbilitySystemComponent->InitAbilityActorInfo(GaspPlayerState, this);
		AttributeSet = GaspPlayerState->GetAttributeSet();

		OnAscInitialized.Broadcast();
	}
}
